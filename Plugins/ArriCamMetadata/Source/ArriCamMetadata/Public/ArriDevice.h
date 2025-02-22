#pragma once

#include "CoreMinimal.h"
#include "ArriLiveLinkConnectionSettings.h"
#include "ArriMetadata.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Logging/LogMacros.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "HAL/Runnable.h"
#include "Misc/Paths.h"
#include "rawMetadata.h"

#include "ArriDevice.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogArri, Log, All);

UCLASS(BlueprintType)
class UArriDevice : public UObject, public FRunnable, public TSharedFromThis<UArriDevice>
{
	GENERATED_BODY()
public:
	UArriDevice();
	virtual ~UArriDevice();

	// Begin FRunnable Interface
	virtual bool Init() override { return true; }
	virtual uint32 Run() override;
	void Start();
	virtual void Stop() override;
	virtual void Exit() override { }

	//Blueprint function returning ARRI Metadata
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get metadata"), Category = "Arri")
	FArriMetadata getMetadata();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Close device"), Category = "Arri")
	void shutdownDevice();

	static FArriMetadata rawToUnrealMetadata(FArriCameraRawMatadata &ArriRawMetadata);
	//static void ParseUDPString(FString str, FArriMetadata& metadata);
	static TSharedPtr<FSocket> createSocket(FArriLiveLinkConnectionSettings connectionSettings, TSharedPtr<FSocket> Socket);
	//static FString StringFromBinaryArray(const TArray<uint8>& BinaryArray);

	//For LiveLink
	FName mSubjectNameCam;

	//Socket information
	TSharedPtr<FSocket> mSocket;
	ISocketSubsystem* mSocketSubsystem;
	FIPv4Endpoint mDeviceEndpoint;
	FIPv4Address mMulticastAddress;
	uint64 mPort = 5432;

	FArriLiveLinkConnectionSettings mConnectionSettings;

	// Arri Metadata
	FArriMetadata mMetadata;

	// Time to wait between attempted receives
	FTimespan mWaitTime = FTimespan::FromMilliseconds(10);

	// Buffer to receive socket data into
	TArray<uint8> mRecvBuffer;
	TMap<FName, FString> mBufferMap;
	FString mBufferString;

	// Threadsafe Bool for terminating the main thread loop
	FThreadSafeBool mStopping;

	// Thread to run socket operations on
	FRunnableThread* mThread;

	// Name of the sockets thread
	FString mThreadName;

	//UMC Flag 
	bool umcErrorThrown = false;

	//Read data from the .uplugin file 
	FString pluginFilePath  = FPaths::ProjectPluginsDir();
	FString pluginDataBuf;
	FPluginMetadata PluginMetadata;


private:
	
};

