// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ILiveLinkSource.h"
#include "Camera/CameraComponent.h"
#include "HAL/Runnable.h"
#include "ILiveLinkClient.h"
#include "Roles/LiveLinkCameraRole.h"
#include "Roles/LiveLinkCameraTypes.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Containers/UnrealString.h"
#include "ArriMetadata.h"
#include "CoreMinimal.h"
#include "ArriLiveLinkConnectionSettings.h"
#include "ArriDevice.h"
//#include "Misc/DateTime.h"

//DECLARE_LOG_CATEGORY_EXTERN(LogArri, Log, All);

class FArriLiveLinkDevice : public ILiveLinkSource, public FRunnable, public TSharedFromThis<FArriLiveLinkDevice>
{
public:
	FArriLiveLinkDevice(const FArriLiveLinkConnectionSettings& ConnectionSettings);
	
	virtual ~FArriLiveLinkDevice();

	// Begin ILiveLinkSource Interface
	virtual void ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid) override;
	virtual bool IsSourceStillValid() const override;
	virtual bool RequestSourceShutdown() override;

	virtual FText GetSourceType() const override;
	virtual FText GetSourceMachineName() const override;
	virtual FText GetSourceStatus() const override;
	// End ILiveLinkSource Interface

	// Begin FRunnable Interface
	virtual bool Init() override { return true; }
	virtual uint32 Run() override;
	void Start();
	virtual void Stop() override;
	virtual void Exit() override { }

private:
	FArriMetadata mMetadata;
	FArriLiveLinkConnectionSettings mConnectionSettings;

	// LiveLink Data
	/** The local client to push data updates to */
	ILiveLinkClient* Client = nullptr;

	/** Our identifier in LiveLink */
	FGuid SourceGuid;

	FText SourceType;
	FText SourceMachineName;
	FText SourceStatus;

	//Socket
	TSharedPtr<FSocket> mSocket;
	ISocketSubsystem* mSocketSubsystem;

	// List of subjects we've already encountered
	TSet<FName> EncounteredSubjects;

	//timecode provider
	FQualifiedFrameTime frameTime;

	// Buffer to receive socket data into
	TArray<uint8> mRecvBuffer;
	TMap<FName, FString> mBufferMap;

	// Time to wait between attempted receives
	FTimespan mWaitTime = FTimespan::FromMilliseconds(10);

	// Threadsafe Bool for terminating the main thread loop
	FThreadSafeBool mStopping;

	// Threadsafe Bool to know when thread as actually stopped
	FThreadSafeBool mStopped;

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

	
};
