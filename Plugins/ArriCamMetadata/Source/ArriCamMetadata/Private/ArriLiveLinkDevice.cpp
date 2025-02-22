// Fill out your copyright notice in the Description page of Project Settings.


#include "ArriLiveLinkDevice.h"

#include "Async/Async.h"
#include "Common/UdpSocketBuilder.h"
#include "Sockets.h"
#include "JsonObjectConverter.h"
#include "SocketSubsystem.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

#define LOCTEXT_NAMESPACE "FArriLiveLinkDevice"

#define RECV_BUFFER_SIZE 1024 * 1024
#define UNIT_FACTOR 100

FArriLiveLinkDevice::FArriLiveLinkDevice(const FArriLiveLinkConnectionSettings& ConnectionSettings)
{
	mSocket = UArriDevice::createSocket(ConnectionSettings, mSocket);
	mConnectionSettings = ConnectionSettings;

	//Get Plugin version from .uplugin file
	pluginFilePath += "ArriCamMetadata/ArriCamMetadata.uplugin";
	FFileHelper::LoadFileToString(pluginDataBuf,*pluginFilePath, FFileHelper::EHashOptions::None);
	FJsonObjectConverter::JsonObjectStringToUStruct<FPluginMetadata>(*pluginDataBuf, &PluginMetadata,0, 0);

}

FArriLiveLinkDevice::~FArriLiveLinkDevice()
{

}

void FArriLiveLinkDevice::ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid)
{
	Client = InClient;
	SourceGuid = InSourceGuid;

	Start();
}

bool FArriLiveLinkDevice::IsSourceStillValid() const
{
	// Source is valid if we have a valid thread and socket
	bool bIsSourceValid = !mStopping && mThread != nullptr && mSocket != nullptr;;
	return bIsSourceValid;
}

bool FArriLiveLinkDevice::RequestSourceShutdown()
{
	Stop();
	if (mThread != nullptr)
	{
		// Verify if our thread has actually stopped processing incoming frames before telling LiveLink we can be shutdown
		if (mStopped == false)
		{
			return false;
		}

		delete mThread;
		mThread = nullptr;
	}
	if (mSocket != nullptr)
	{
		mSocket->Close();
		mSocket = nullptr;
		UE_LOG(LogArri, Log, TEXT("Socket closed"));
	}

	Client = nullptr;
	EncounteredSubjects.Empty();
	SourceGuid.Invalidate();
	mStopping = false;
	return true;
}

void FArriLiveLinkDevice::Start()
{
	mSocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	mRecvBuffer.SetNumUninitialized(RECV_BUFFER_SIZE);

	mThreadName = "LiveLink Receiver";
	mThreadName.AppendInt(FAsyncThreadIndex::GetNext());
	mThread = FRunnableThread::Create(this, *mThreadName, 128 * 1024, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());

}

void FArriLiveLinkDevice::Stop()
{
	mStopping = true;
}

uint32 FArriLiveLinkDevice::Run()
{
	mStopped = false;
	TSharedRef<FInternetAddr> Sender = mSocketSubsystem->CreateInternetAddr();

	while (!mStopping && mSocket != nullptr)
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
						if (!Client)
						{
							continue;
						}
						TArray<char> ReceivedData;
						ReceivedData.Reserve(Read);
						memcpy(ReceivedData.GetData(), mRecvBuffer.GetData(), Read);
						FString ReceivedBuf(Read, ReceivedData.GetData());

						FString Address = Sender->ToString(false);
						FString sCam = mConnectionSettings.SubjectName;
						sCam += "@";
						sCam += Address;
						FName SubjectNameCam(sCam);

						bool bCreateSubject = EncounteredSubjects.Contains(SubjectNameCam);

						if (!bCreateSubject)
						{
							mBufferMap.Add(SubjectNameCam, ReceivedBuf);
							EncounteredSubjects.Add(SubjectNameCam);
							bCreateSubject = false;

							FLiveLinkStaticDataStruct StaticDataStruct = FLiveLinkStaticDataStruct(FLiveLinkCameraStaticData::StaticStruct());
							FLiveLinkCameraStaticData& StaticData = *StaticDataStruct.Cast<FLiveLinkCameraStaticData>();

							StaticData.bIsFocalLengthSupported = true;
							StaticData.bIsFocusDistanceSupported = true;
							StaticData.bIsApertureSupported = true;

							Client->PushSubjectStaticData_AnyThread({ SourceGuid, SubjectNameCam }, ULiveLinkCameraRole::StaticClass(), MoveTemp(StaticDataStruct));
							UE_LOG(LogArri, Log, TEXT("Subject created with name: %s"), *FString(SubjectNameCam.ToString()));
						}
						else
						{
							mBufferMap[SubjectNameCam].Append(ReceivedBuf);
						}

						FArriCameraRawMatadata ArriRawMetadata;
						FJsonObjectConverter::JsonObjectStringToUStruct<FArriCameraRawMatadata>(ReceivedBuf, &ArriRawMetadata, 0, 0);


						mMetadata = UArriDevice::rawToUnrealMetadata(ArriRawMetadata);

						
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
								UE_LOG(LogArri, Error, TEXT("Your current UMC4 version is %s, which is incompatible with the plugin version %s, please update the UMC4 to version 2.3.2645"), *mMetadata.umcSoftwareVersion, *PluginMetadata.versionName);
								umcErrorThrown = true;
							}
		
						}



						FLiveLinkFrameDataStruct CameraFrameDataStruct = FLiveLinkFrameDataStruct(FLiveLinkCameraFrameData::StaticStruct());
						FLiveLinkCameraFrameData& CameraFrameData = *CameraFrameDataStruct.Cast<FLiveLinkCameraFrameData>();

						FLiveLinkBaseDataStruct<FLiveLinkBaseBlueprintData> BaseDataStruct;

						frameTime = FQualifiedFrameTime(mMetadata.timecode.aquisitionTimestamp, mMetadata.timecode.timebase);

						CameraFrameData.MetaData.SceneTime = frameTime;
						CameraFrameData.Aperture = mMetadata.aperture;
						CameraFrameData.FocalLength = mMetadata.focalLength;
						CameraFrameData.FocusDistance = mMetadata.focusDistanceMetric;

						CameraFrameData.MetaData.StringMetaData.Add("CameraHealth", UEnum::GetValueAsString(mMetadata.cameraState.health));
						CameraFrameData.MetaData.StringMetaData.Add("CameraMajorState", UEnum::GetValueAsString(mMetadata.cameraState.majorState));

						CameraFrameData.MetaData.StringMetaData.Add("ExposureIndex", FString::FromInt(mMetadata.exposureIndex));
						CameraFrameData.MetaData.StringMetaData.Add("ExposureTime", FString::SanitizeFloat(mMetadata.exposureTime));
						CameraFrameData.MetaData.StringMetaData.Add("ShutterAngle", FString::SanitizeFloat(mMetadata.shutterAngle));
						CameraFrameData.MetaData.StringMetaData.Add("NDDensity", FString::SanitizeFloat(mMetadata.ndFilterDensity));
						CameraFrameData.MetaData.StringMetaData.Add("CCT", FString::FromInt(mMetadata.colorTemperature));
						CameraFrameData.MetaData.StringMetaData.Add("tint", FString::SanitizeFloat(mMetadata.whiteBalanceTint));
						CameraFrameData.MetaData.StringMetaData.Add("Roll", FString::SanitizeFloat(mMetadata.roll));
						CameraFrameData.MetaData.StringMetaData.Add("Tilt", FString::SanitizeFloat(mMetadata.tilt));
						CameraFrameData.MetaData.StringMetaData.Add("ClipName", mMetadata.clipName);

						Client->PushSubjectFrameData_AnyThread({ SourceGuid, SubjectNameCam }, MoveTemp(CameraFrameDataStruct));
						mBufferMap[SubjectNameCam].Empty();
					}
				}
			}
		}
	}
	mStopped = true;
	return 0;
}

FText FArriLiveLinkDevice::GetSourceMachineName() const
{
	return FText().FromString(FPlatformProcess::ComputerName());
}

FText FArriLiveLinkDevice::GetSourceStatus() const
{
	return LOCTEXT("ArriLiveLinkStatus", "Active");
}

FText FArriLiveLinkDevice::GetSourceType() const
{
	return LOCTEXT("ArriLiveLinkSourceType", "Arri Live Link controller");
}

#undef LOCTEXT_NAMESPACE