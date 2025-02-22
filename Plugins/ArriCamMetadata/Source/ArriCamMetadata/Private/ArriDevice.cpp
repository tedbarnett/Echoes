#include "ArriDevice.h"
#include "Common/UdpSocketBuilder.h"
#include "JsonObjectConverter.h"
#include "Async/Async.h"
#include "HAL/RunnableThread.h"
#include "Misc/FileHelper.h"

//#include <string>

#define RECV_BUFFER_SIZE 1024 * 1024
#define UNIT_FACTOR 100

DEFINE_LOG_CATEGORY(LogArri);


UArriDevice::UArriDevice() 
{
	//Get Plugin version from .uplugin file
	pluginFilePath += "ArriCamMetadata/ArriCamMetadata.uplugin";
	FFileHelper::LoadFileToString(pluginDataBuf,*pluginFilePath, FFileHelper::EHashOptions::None);
	FJsonObjectConverter::JsonObjectStringToUStruct<FPluginMetadata>(*pluginDataBuf, &PluginMetadata,0, 0);
}

UArriDevice::~UArriDevice()
{
	shutdownDevice();
}

TSharedPtr<FSocket> UArriDevice::createSocket(FArriLiveLinkConnectionSettings connectionSettings, TSharedPtr<FSocket> Socket)
{
	FSocket* SocketPtr = nullptr;
	bool createSocket = true;
	FString SocketAddr;

	FIPv4Endpoint endpoint;
	FIPv4Address endpointAddress;

	//Wildcard to find IP addresses in a designated subnet
	FString Wildcard = "*";
	if (connectionSettings.IPAddress.Contains(Wildcard))
	{
		TArray<FString> IPAddrSegments;
		connectionSettings.IPAddress.ParseIntoArray(IPAddrSegments, TEXT("."), true);

		endpointAddress.A = FCString::Atoi(*IPAddrSegments[0]);
		endpointAddress.B = FCString::Atoi(*IPAddrSegments[1]);
		endpointAddress.C = FCString::Atoi(*IPAddrSegments[2]);

		TArray<TSharedPtr<FInternetAddr>> localAddresses;
		bool gotAddresses = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalAdapterAddresses(localAddresses);
		bool foundAddress = false;

		if (gotAddresses)
		{
			int numOfAddresses = localAddresses.Num();
			TArray<FString> localAddressesAsString;

			for (int i = 0; i < numOfAddresses; i++)
			{
				localAddressesAsString.Add(*localAddresses[i].Get()->ToString(false));
				UE_LOG(LogArri, Log, TEXT("Found local address %d : %s"), i, *localAddressesAsString[i]);
			}

			int IPCounter = 1;
			while (!foundAddress && IPCounter <= 255)
			{
				endpointAddress.D = IPCounter;
				if (localAddressesAsString.Contains(endpointAddress.ToString()))
				{
					foundAddress = true;
					UE_LOG(LogArri, Log, TEXT("Added local address via Wildcard @ : %s"), *endpointAddress.ToString());
				}
				else
				{
					IPCounter++;
				}
			}
		}
		else
		{
			UE_LOG(LogArri, Error, TEXT("Could not find any connected adapters. Make sure you are connected to a network"));
		}
		
	}
	else
	{
		FIPv4Address::Parse(connectionSettings.IPAddress, endpointAddress);		
	}

	endpoint.Address = endpointAddress;
	endpoint.Port = connectionSettings.UDPPortNumber;

	UE_LOG(LogArri, Log, TEXT("Device endpoint configured to address: %s"), *endpointAddress.ToString());
	UE_LOG(LogArri, Log, TEXT("Device endpoint configured to port: %s"), *FString::FromInt(connectionSettings.UDPPortNumber));


	if (connectionSettings.Mode == UDPMode::MULTI)
	{
		FIPv4Address multicastAddress;
		FIPv4Address::Parse(connectionSettings.MulticastAddress, multicastAddress);

		if (!multicastAddress.IsOrganizationLocalMulticast())
		{
			UE_LOG(LogArri, Error, TEXT("Multicast address must be in organization local scope of 239.192.x.x, current address is: %s. Failed to create socket. Please delete source and add again with correct multicast Address."), *connectionSettings.MulticastAddress);
			createSocket = false;
		}
		else
		{
			UE_LOG(LogArri, Log, TEXT("Socket configured to multicast address: %s"), *multicastAddress.ToString());
			SocketPtr = FUdpSocketBuilder(TEXT("UDPSOCKET"))
				.AsNonBlocking()
				.AsReusable()
				.BoundToPort(connectionSettings.UDPPortNumber)
				.BoundToEndpoint(endpoint)
				.WithReceiveBufferSize(RECV_BUFFER_SIZE)
				.JoinedToGroup(multicastAddress, endpoint.Address)
				.WithMulticastLoopback()
				.WithMulticastTtl(2);
		}
	}
	else if (connectionSettings.Mode == UDPMode::UNI)
	{
		UE_LOG(LogArri, Log, TEXT("Socket configured to unicast"));
		SocketPtr = FUdpSocketBuilder(TEXT("UDPSOCKET"))
			.AsNonBlocking()
			.AsReusable()
			.BoundToPort(connectionSettings.UDPPortNumber)
			.BoundToEndpoint(endpoint)
			.WithReceiveBufferSize(RECV_BUFFER_SIZE);
	}
	if (SocketPtr != nullptr)
	{
		TSharedPtr<FInternetAddr> SocketAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		
		SocketPtr->SetReuseAddr(true);
		SocketPtr->GetAddress(*SocketAddress);
		SocketAddr = SocketAddress->ToString(false);
		UE_LOG(LogArri, Log, TEXT("Socket address is confirmed: %s"), *SocketAddr);
	}
	else
	{
		UE_LOG(LogArri, Error, TEXT("Could not create socket, please check whether you are using the correct IP of your preferred network card or whether the port is blocked by a different process."), *SocketAddr);
	}

	if (SocketAddr != endpointAddress.ToString())
	{
		UE_LOG(LogArri, Warning, TEXT("Socket address does not match endpoint address: socket: %s , endpoint: %s"), *SocketAddr, *endpointAddress.ToString());
	}
	else
	{
		UE_LOG(LogArri, Log, TEXT("Socket address matches andpoint: socket: %s , endpoint: %s"), *SocketAddr, *endpointAddress.ToString());
	}
	if ((SocketPtr != nullptr) && (SocketPtr->GetSocketType() == SOCKTYPE_Datagram))
	{
		UE_LOG(LogArri, Log, TEXT("Socket created sucessfully for ARRI Live Link Source"));
	}
	else
	{
		UE_LOG(LogArri, Warning, TEXT("Could not create socket for ARRI Live Link Source"));
	}

	Socket = MakeShareable(SocketPtr);
	return Socket;
}

void UArriDevice::Start()
{
	mSocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	mRecvBuffer.SetNumUninitialized(RECV_BUFFER_SIZE);

	mThreadName = "UDP Receiver";
	mThreadName.AppendInt(FAsyncThreadIndex::GetNext());
	mThread = FRunnableThread::Create(this, *mThreadName, 128 * 1024, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());
}

void UArriDevice::Stop()
{
	mStopping = true;
}

uint32 UArriDevice::Run()
{
	TSharedRef<FInternetAddr> Sender = mSocketSubsystem->CreateInternetAddr();
	while (!mStopping)
	{
		if (mSocket->Wait(ESocketWaitConditions::WaitForRead, mWaitTime))
		{
			uint32 Size;
			while (mSocket->HasPendingData(Size))
			{
				int32 Read = 0;
				if (mSocket->RecvFrom(mRecvBuffer.GetData(), mRecvBuffer.Num(), Read, *Sender))
				{
					if (Read > 0)
					{
						TArray<char> ReceivedData;
						ReceivedData.Reserve(Read);
						memcpy(ReceivedData.GetData(), mRecvBuffer.GetData(), Read);
						FString ReceivedBuf(Read, ReceivedData.GetData());

						FArriCameraRawMatadata ArriRawMetadata;
						FJsonObjectConverter::JsonObjectStringToUStruct<FArriCameraRawMatadata>(ReceivedBuf, &ArriRawMetadata,0, 0);

						FString Address = Sender->ToString(false);
						FString sCam = FString("Arri-");
						sCam += "UMC-4@";
						sCam += Address;
						mSubjectNameCam = FName(sCam);

						UE_LOG(LogArri, Verbose, TEXT("Receiving data from: %s"), *sCam);


						mMetadata = rawToUnrealMetadata(ArriRawMetadata);

						//throw error if the umc version is not 2.3 and set 
						if(mMetadata.umcSoftwareVersion != "2.3" && (mMetadata.cameraModel != "ALEXA 35")) 
						{
							//Set umc manually if it is 2.2 because the received string has a different structure
							if(ReceivedBuf.Contains("\"umc4SoftwareVersion\":\"2.2\"")){
							mMetadata.umcSoftwareVersion = "2.2";
							}

							//throw error only once
							if(!umcErrorThrown)
							{
								UE_LOG(LogArri, Error, TEXT("Your current UMC4 version is %s, which is incompatible with the plugin version %s, please update the UMC4 to version 2.3.2645."), *mMetadata.umcSoftwareVersion, *PluginMetadata.versionName);
								umcErrorThrown = true;
							}
		
						}
					}
				}
			}
		}
	}
	return 0;
}

FArriMetadata UArriDevice::rawToUnrealMetadata(FArriCameraRawMatadata &ArriRawMetadata)
{
	FArriMetadata unrealMetadata;

	FString delimiter = "/";


	//Map string to UENUM for Camera health
	if (ArriRawMetadata.camera.state.health == "good")
	{
		unrealMetadata.cameraState.health = FArriCameraHealthState::GOOD;
	}
	else if (ArriRawMetadata.camera.state.health == "error")
	{
		unrealMetadata.cameraState.health = FArriCameraHealthState::ERROR;
	}
	else if (ArriRawMetadata.camera.state.health == "warning")
	{
		unrealMetadata.cameraState.health = FArriCameraHealthState::WARNING;
	}
	else
	{
		unrealMetadata.cameraState.health = FArriCameraHealthState::UNKNOWN;
	}

	UE_LOG(LogArri, Verbose, TEXT("Camera health: %s"), *ArriRawMetadata.camera.state.health);

	//Map string to UENUM for Camera Major State
	if (ArriRawMetadata.camera.state.majorState == "IDLE")
	{
		unrealMetadata.cameraState.majorState = FArriCameraMajorState::CS_IDLE;
	}
	else if (ArriRawMetadata.camera.state.majorState == "STANDBY")
	{
		unrealMetadata.cameraState.majorState = FArriCameraMajorState::CS_STANDBY;
	}
	else if (ArriRawMetadata.camera.state.majorState == "RECORDING_START")
	{
		unrealMetadata.cameraState.majorState = FArriCameraMajorState::CS_REC_START;
	}
	else if (ArriRawMetadata.camera.state.majorState == "RECORDING")
	{
		unrealMetadata.cameraState.majorState = FArriCameraMajorState::CS_RECORDING;
	}
	else if (ArriRawMetadata.camera.state.majorState == "RECORDING_STOP")
	{
		unrealMetadata.cameraState.majorState = FArriCameraMajorState::CS_REC_STOP;
	}
	else if (ArriRawMetadata.camera.state.majorState == "PREREC_START")
	{
		unrealMetadata.cameraState.majorState = FArriCameraMajorState::CS_PREREC_START;
	}
	else if (ArriRawMetadata.camera.state.majorState == "PREREC")
	{
		unrealMetadata.cameraState.majorState = FArriCameraMajorState::CS_PRE_RECORDING;
	}
	else if (ArriRawMetadata.camera.state.majorState == "PREREC_STOP")
	{
		unrealMetadata.cameraState.majorState = FArriCameraMajorState::CS_PREREC_STOP;
	}
	else if (ArriRawMetadata.camera.state.majorState == "PLAYBACK")
	{
		unrealMetadata.cameraState.majorState = FArriCameraMajorState::CS_PLAYBACK;
	}
	else if (ArriRawMetadata.camera.state.majorState == "ERASE")
	{
		unrealMetadata.cameraState.majorState = FArriCameraMajorState::CS_ERASE;
	}
	else if (ArriRawMetadata.camera.state.majorState == "DELETE")
	{
		unrealMetadata.cameraState.majorState = FArriCameraMajorState::CS_DELETE;
	}
	else if (ArriRawMetadata.camera.state.majorState == "INITIALIZE")
	{
		unrealMetadata.cameraState.majorState = FArriCameraMajorState::CS_INITIALIZE;
	}
	else if (ArriRawMetadata.camera.state.majorState == "SHUTDOWN")
	{
		unrealMetadata.cameraState.majorState = FArriCameraMajorState::CS_SHUTDOWN;
	}
	else if (ArriRawMetadata.camera.state.majorState == "UPDATE")
	{
		unrealMetadata.cameraState.majorState = FArriCameraMajorState::CS_UPDATE;
	}

	UE_LOG(LogArri, Verbose, TEXT("Camera major state: %s"), *ArriRawMetadata.camera.state.majorState);

	unrealMetadata.umcSerialNumber = ArriRawMetadata.camera.device.umc4SerialNumber;
	UE_LOG(LogArri, Verbose, TEXT("UMC serial number: %s"), *unrealMetadata.umcSerialNumber);
	unrealMetadata.umcSoftwareVersion = ArriRawMetadata.camera.device.umc4SoftwareVersion;
	UE_LOG(LogArri, Verbose, TEXT("UMC software version: %s"), *unrealMetadata.umcSoftwareVersion);

	unrealMetadata.cameraModel = ArriRawMetadata.camera.device.cameraModel;
	UE_LOG(LogArri, Verbose, TEXT("Camera model: %s"), *unrealMetadata.cameraModel);
	unrealMetadata.cameraSerial = ArriRawMetadata.camera.device.cameraSerialNumber;
	UE_LOG(LogArri, Verbose, TEXT("Camera serial number: %s"), *unrealMetadata.cameraSerial);
	unrealMetadata.cameraSoftwarePackageName = ArriRawMetadata.camera.device.cameraSoftwarePackageName;
	UE_LOG(LogArri, Verbose, TEXT("Camera software package name: %s"), *unrealMetadata.cameraSoftwarePackageName);
	unrealMetadata.cameraSoftwarePackageVersion = ArriRawMetadata.camera.device.cameraSoftwarePackageVersion;
	UE_LOG(LogArri, Verbose, TEXT("Camera software package version: %s"), *unrealMetadata.cameraSoftwarePackageVersion);
	unrealMetadata.cameraVariant = ArriRawMetadata.camera.device.cameraVariant;
	UE_LOG(LogArri, Verbose, TEXT("Camera variant: %s"), *unrealMetadata.cameraVariant);

	unrealMetadata.ndFilterDensity = ArriRawMetadata.camera.optic.filter.ndFilterDensity;
	UE_LOG(LogArri, Verbose, TEXT("ND filter density: %f"), unrealMetadata.ndFilterDensity);

	//Lens State Metadata
	unrealMetadata.circleOfConfusion = ArriRawMetadata.camera.optic.lens.lensDevice.circleOfConfusion;
	UE_LOG(LogArri, Verbose, TEXT("Circle of confusion: %f"), unrealMetadata.circleOfConfusion);
	unrealMetadata.lensLimitsFocusImperial.min = ArriRawMetadata.camera.optic.lens.lensDevice.lensLimitsFocusImperial.min;
	unrealMetadata.lensLimitsFocusImperial.max = ArriRawMetadata.camera.optic.lens.lensDevice.lensLimitsFocusImperial.max;
	UE_LOG(LogArri, Verbose, TEXT("Lens limits focus imperial min: %d, max: %d"), unrealMetadata.lensLimitsFocusImperial.min, unrealMetadata.lensLimitsFocusImperial.max);
	unrealMetadata.lensLimitsFocusMetric.min = ArriRawMetadata.camera.optic.lens.lensDevice.lensLimitsFocusMetric.min;
	unrealMetadata.lensLimitsFocusMetric.max = ArriRawMetadata.camera.optic.lens.lensDevice.lensLimitsFocusMetric.max;
	UE_LOG(LogArri, Verbose, TEXT("Lens limits focus metric min: %d, max: %d"), unrealMetadata.lensLimitsFocusMetric.min, unrealMetadata.lensLimitsFocusMetric.max);
	unrealMetadata.lensLimitsIris.min = ArriRawMetadata.camera.optic.lens.lensDevice.lensLimitsIris.min;
	unrealMetadata.lensLimitsIris.max = ArriRawMetadata.camera.optic.lens.lensDevice.lensLimitsIris.max;
	UE_LOG(LogArri, Verbose, TEXT("Lens limits iris min: %d, max: %d"), unrealMetadata.lensLimitsIris.min, unrealMetadata.lensLimitsIris.max);
	unrealMetadata.lensLimitsZoom.min = ArriRawMetadata.camera.optic.lens.lensDevice.lensLimitsZoom.min;
	unrealMetadata.lensLimitsZoom.max = ArriRawMetadata.camera.optic.lens.lensDevice.lensLimitsZoom.max;
	UE_LOG(LogArri, Verbose, TEXT("Lens limits zoom min: %d, max: %d"), unrealMetadata.lensLimitsIris.min, unrealMetadata.lensLimitsIris.max);
	unrealMetadata.lensModel = ArriRawMetadata.camera.optic.lens.lensDevice.lensModel;
	UE_LOG(LogArri, Verbose, TEXT("Lens name: %s"), *unrealMetadata.lensModel);
	unrealMetadata.motorRawEncoderLimitsFocus.min = ArriRawMetadata.camera.optic.lens.lensDevice.lensMotorRawEncoderLimitsFocus.min;
	unrealMetadata.motorRawEncoderLimitsFocus.max = ArriRawMetadata.camera.optic.lens.lensDevice.lensMotorRawEncoderLimitsFocus.max;
	UE_LOG(LogArri, Verbose, TEXT("Motor raw encoder limits focus min: %d, max: %d"), unrealMetadata.motorRawEncoderLimitsFocus.min, unrealMetadata.motorRawEncoderLimitsFocus.max);
	unrealMetadata.motorRawEncoderLimitsFocalLength.min = ArriRawMetadata.camera.optic.lens.lensDevice.lensMotorRawEncoderLimitsFL.min;
	unrealMetadata.motorRawEncoderLimitsFocalLength.max = ArriRawMetadata.camera.optic.lens.lensDevice.lensMotorRawEncoderLimitsFL.max;
	UE_LOG(LogArri, Verbose, TEXT("Motor raw encoder limits focal Length min: %d, max: %d"), unrealMetadata.motorRawEncoderLimitsFocalLength.min, unrealMetadata.motorRawEncoderLimitsFocalLength.max);
	unrealMetadata.motorRawEncoderLimitsIris.min = ArriRawMetadata.camera.optic.lens.lensDevice.lensMotorRawEncoderLimitsIris.min;
	unrealMetadata.motorRawEncoderLimitsIris.max = ArriRawMetadata.camera.optic.lens.lensDevice.lensMotorRawEncoderLimitsIris.max;
	UE_LOG(LogArri, Verbose, TEXT("Motor raw encoder limits iris min: %d, max: %d"), unrealMetadata.motorRawEncoderLimitsIris.min, unrealMetadata.motorRawEncoderLimitsIris.max);
	unrealMetadata.lensScaleRawEncoderSource = ArriRawMetadata.camera.optic.lens.lensDevice.lensScaleRawEncoderSource;
	UE_LOG(LogArri, Verbose, TEXT("Lens scale raw encoder source: %s"), *unrealMetadata.lensScaleRawEncoderSource);
	unrealMetadata.lensSerialNumber = ArriRawMetadata.camera.optic.lens.lensDevice.serialNumber;
	UE_LOG(LogArri, Verbose, TEXT("Lens serial: %s"), *unrealMetadata.lensSerialNumber);

	//lensSequeeze is not available from UMC4. So do not call this if this metadate is empty
	if (!ArriRawMetadata.camera.optic.lens.lensDevice.lensSqueezeFactor.IsEmpty())
	{
		FString lensSqueezeNumerator;
		FString lensSqueezeDenominator;
		ArriRawMetadata.camera.optic.lens.lensDevice.lensSqueezeFactor.Split(delimiter, &lensSqueezeNumerator, &lensSqueezeDenominator);
		unrealMetadata.lensSqueezeFactor = FCString::Atoi(*lensSqueezeNumerator) / FCString::Atoi(*lensSqueezeDenominator);
		UE_LOG(LogArri, Verbose, TEXT("Lens squeeze factor: %f"), unrealMetadata.lensSqueezeFactor);
	}

	unrealMetadata.effectiveFocalLength = ArriRawMetadata.camera.optic.lens.lensState.lensEffectiveFocalLength;
	UE_LOG(LogArri, Verbose, TEXT("Effective focal length: %d"), unrealMetadata.effectiveFocalLength);
	unrealMetadata.entrancePupilOffset = ArriRawMetadata.camera.optic.lens.lensState.lensEntrancePupilOffset;
	UE_LOG(LogArri, Verbose, TEXT("Entrance pupil offset: %d"), unrealMetadata.entrancePupilOffset);
	unrealMetadata.focalLength = float(ArriRawMetadata.camera.optic.lens.lensState.lensFocalLength) / 1000;
	UE_LOG(LogArri, Verbose, TEXT("Focal length in mm: %f"), unrealMetadata.focalLength);
	//Tested on LDS wit ALEXA35. Both values are always present in the metadata. No need for conversion.
	//TODO: Check with UMC4! There might be a need for some sort of handling when using dumb lenses (LDA/LDM)
	if (ArriRawMetadata.camera.optic.lens.lensState.lensFocusDistanceMetric == -1)
	{
		unrealMetadata.focusDistanceMetric = 100000.0;
		UE_LOG(LogArri, Verbose, TEXT("Focus distance in cm: %f"), unrealMetadata.focusDistanceMetric);
	}
	else
	{
		unrealMetadata.focusDistanceMetric = float(ArriRawMetadata.camera.optic.lens.lensState.lensFocusDistanceMetric) / 10;
		UE_LOG(LogArri, Verbose, TEXT("Focus distance in cm: %f"), unrealMetadata.focusDistanceMetric);
	}
	
	unrealMetadata.focusDistanceImperial = float(ArriRawMetadata.camera.optic.lens.lensState.lensFocusDistanceImperial)/1000;
	UE_LOG(LogArri, Verbose, TEXT("Focus distance in inches: %f"), unrealMetadata.focusDistanceImperial);
	unrealMetadata.aperture = pow(2.0, ((float(ArriRawMetadata.camera.optic.lens.lensState.lensIris) / 1000) - 1) / 2);
	UE_LOG(LogArri, Verbose, TEXT("Aperture: %f"), unrealMetadata.aperture);

	unrealMetadata.lensMotorRawEncoderValueFocalLength = ArriRawMetadata.camera.optic.lens.lensState.lensMotorRawEncoderValueFl;
	UE_LOG(LogArri, Verbose, TEXT("Raw encoder focal length from motor: %d"), unrealMetadata.lensMotorRawEncoderValueFocalLength);
	unrealMetadata.lensMotorRawEncoderValueFocus = ArriRawMetadata.camera.optic.lens.lensState.lensMotorRawEncoderValueFocus;
	UE_LOG(LogArri, Verbose, TEXT("Raw encoder focus from motor: %d"), unrealMetadata.lensMotorRawEncoderValueFocus);
	unrealMetadata.lensMotorRawEncoderValueIris = ArriRawMetadata.camera.optic.lens.lensState.lensMotorRawEncoderValueIris;
	UE_LOG(LogArri, Verbose, TEXT("Raw encoder iris from motor: %d"), unrealMetadata.lensMotorRawEncoderValueIris);

	unrealMetadata.lensScaleRawEncoderValueFocalLength = ArriRawMetadata.camera.optic.lens.lensState.lensScaleRawEncoderValueFl;
	UE_LOG(LogArri, Verbose, TEXT("Lens scale raw encoder focal length: %d"), unrealMetadata.lensScaleRawEncoderValueFocalLength);
	unrealMetadata.lensScaleRawEncoderValueFocus = ArriRawMetadata.camera.optic.lens.lensState.lensScaleRawEncoderValueFocus;
	UE_LOG(LogArri, Verbose, TEXT("Lens scale raw encoder focus: %d"), unrealMetadata.lensScaleRawEncoderValueFocus);
	unrealMetadata.lensScaleRawEncoderValueIris = ArriRawMetadata.camera.optic.lens.lensState.lensScaleRawEncoderValueIris;
	UE_LOG(LogArri, Verbose, TEXT("Lens scale raw encoder iris: %d"), unrealMetadata.lensScaleRawEncoderValueIris);

	//lens converter
	unrealMetadata.lensConverter.converterFocalLenMultiplier = ArriRawMetadata.camera.optic.converter.converterFocalLenMultiplier;
	UE_LOG(LogArri, Verbose, TEXT("Lens converter focal length multiplier: %f"), unrealMetadata.lensConverter.converterFocalLenMultiplier);
	unrealMetadata.lensConverter.converterLightLossFactor = ArriRawMetadata.camera.optic.converter.converterLightLossFactor;
	UE_LOG(LogArri, Verbose, TEXT("Lens converter light loss factor: %f"), unrealMetadata.lensConverter.converterLightLossFactor);
	unrealMetadata.lensConverter.converterModel = ArriRawMetadata.camera.optic.converter.converterModel;
	UE_LOG(LogArri, Verbose, TEXT("Lens converter model: %s"),*unrealMetadata.lensConverter.converterModel);
	unrealMetadata.lensConverter.converterPhysicalLength = ArriRawMetadata.camera.optic.converter.converterPhysicalLength;
	UE_LOG(LogArri, Verbose, TEXT("Lens converter physical length: %d"), unrealMetadata.lensConverter.converterPhysicalLength);
	unrealMetadata.lensConverter.converterSerialNumber = ArriRawMetadata.camera.optic.converter.converterSerialNumber;
	UE_LOG(LogArri, Verbose, TEXT("Lens converter serial number: %s"), *unrealMetadata.lensConverter.converterSerialNumber);

	//Pan / Roll / tilt. Pan is only available from Alexa35
	unrealMetadata.pan = ArriRawMetadata.camera.positional.orientation.pan;
	UE_LOG(LogArri, Verbose, TEXT("Pan: %f"), unrealMetadata.roll);
	unrealMetadata.roll = ArriRawMetadata.camera.positional.orientation.roll;
	UE_LOG(LogArri, Verbose, TEXT("Roll: %f"), unrealMetadata.roll);
	unrealMetadata.tilt = ArriRawMetadata.camera.positional.orientation.tilt;
	UE_LOG(LogArri, Verbose, TEXT("Tilt: %f"), unrealMetadata.tilt);

	unrealMetadata.colorTemperature = ArriRawMetadata.camera.processing.whiteBalance.colorTemperature;
	UE_LOG(LogArri, Verbose, TEXT("Color temperature: %d"), unrealMetadata.colorTemperature);
	unrealMetadata.whiteBalanceTint = ArriRawMetadata.camera.processing.whiteBalance.whiteBalanceTint;
	UE_LOG(LogArri, Verbose, TEXT("White balance tint: %f"), unrealMetadata.whiteBalanceTint);

	//Generate Timecode from Metadata stream with project framerate
	FString timebaseFramerateNumerator;
	FString timebaseFramerateDenominator;
	ArriRawMetadata.camera.recording.projectRate.timebase.Split(delimiter, &timebaseFramerateNumerator, &timebaseFramerateDenominator);
	//Project framerate
	unrealMetadata.timecode.timebase.Numerator = FCString::Atoi(*timebaseFramerateNumerator);
	unrealMetadata.timecode.timebase.Denominator = FCString::Atoi(*timebaseFramerateDenominator);
	UE_LOG(LogArri, Verbose, TEXT("Project frame rate: %f"), unrealMetadata.timecode.timebase.AsDecimal());

	//Sensor Framerate
	FString sensorFramerateNumerator;
	FString sensorFramerateDenominator;
	ArriRawMetadata.camera.sensor.sensorState.sensorSampleRate.Split(delimiter, &sensorFramerateNumerator, &sensorFramerateDenominator);
	unrealMetadata.sensorSampleRate.Numerator = FCString::Atoi(*sensorFramerateNumerator);
	unrealMetadata.sensorSampleRate.Denominator = FCString::Atoi(*sensorFramerateDenominator);
	UE_LOG(LogArri, Verbose, TEXT("Sensor frame rate: %f"), unrealMetadata.sensorSampleRate.AsDecimal());

	//timecode relevant metadata
	unrealMetadata.timecode.dropFrame = ArriRawMetadata.camera.recording.projectRate.dropframe;
	UE_LOG(LogArri, Verbose, TEXT("Drop frame: %s"), *FString(unrealMetadata.timecode.dropFrame ? TEXT("true") : TEXT("false")));
	unrealMetadata.timecode.aquisitionTimestamp.FrameNumber = ArriRawMetadata.camera.recording.acquisitionTimestamp;
	UE_LOG(LogArri, Verbose, TEXT("Frame number: %d"), unrealMetadata.timecode.aquisitionTimestamp.FrameNumber.Value);

	//ClipName & Clipnumber
	//TODO: Chack for accuracy between UMC4 and Alexa35
	unrealMetadata.clipName = ArriRawMetadata.camera.recording.clip.slateInfo.clipName;
	UE_LOG(LogArri, Verbose, TEXT("Clip name: %s"), *unrealMetadata.clipName);

	unrealMetadata.clipNumber = ArriRawMetadata.camera.recording.clip.slateInfo.clipNumber;
	UE_LOG(LogArri, Verbose, TEXT("Clip number: %d"), unrealMetadata.clipNumber);

	unrealMetadata.averagedFrames = ArriRawMetadata.camera.sensor.sensorState.averagedFrames;
	UE_LOG(LogArri, Verbose, TEXT("Averaged sensor frames: %d"), unrealMetadata.averagedFrames);
	unrealMetadata.exposureIndex = ArriRawMetadata.camera.sensor.sensorState.exposureIndex;
	UE_LOG(LogArri, Verbose, TEXT("Exposure index: %d"), unrealMetadata.exposureIndex);

	FString exposureTimeNumerator;
	FString exposureTimeDenominator;
	ArriRawMetadata.camera.sensor.sensorState.exposureTime.Split(delimiter, &exposureTimeNumerator, &exposureTimeDenominator);

	unrealMetadata.exposureTime = float(FCString::Atoi(*exposureTimeNumerator)) / float(FCString::Atoi(*exposureTimeDenominator));
	UE_LOG(LogArri, Verbose, TEXT("Exposure time in seconds: %f"), unrealMetadata.exposureTime);

	unrealMetadata.shutterAngle = (unrealMetadata.sensorSampleRate.AsDecimal() * 360.0) / float((FCString::Atoi(*exposureTimeDenominator)) / 1000.0);
	UE_LOG(LogArri, Verbose, TEXT("Shutter angle in degrees: %f"), unrealMetadata.shutterAngle);

	unrealMetadata.mediumCapacity = ArriRawMetadata.camera.recording.medium.mediumCapacity;
	UE_LOG(LogArri, Verbose, TEXT("Resuming media capacity in seconds: %d"), unrealMetadata.mediumCapacity);

	return unrealMetadata;
}

void UArriDevice::shutdownDevice()
{
	Stop();
	if (mThread != nullptr)
	{
		mThread->WaitForCompletion();
		delete mThread;
		mThread = nullptr;
	}
	if (mSocket != nullptr)
	{
		mSocket->Close();
		mSocket = nullptr;
		UE_LOG(LogArri, Log, TEXT("Socket closed"));
	}
	mBufferMap.Empty();
	mStopping = false;
}

FArriMetadata UArriDevice::getMetadata()
{
	return mMetadata;
}