#pragma once

#include "CoreMinimal.h"
#include "ArriLiveLinkConnectionSettings.generated.h"

UENUM(BlueprintType)
enum class UDPMode : uint8
{
	MULTI UMETA(DisplayName = "Multicast Mode"),
	UNI   UMETA(DisplayName = "Unicast Mode")
};

USTRUCT()
struct FArriLiveLinkConnectionSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Connection Settings")
	UDPMode Mode = UDPMode::MULTI;

	UPROPERTY(EditAnywhere, Category = "Connection Settings")
	FString SubjectName = TEXT("UMC-4");

	/** Endpoint IP Address */
	UPROPERTY(EditAnywhere, Category = "Connection Settings")
	FString IPAddress = TEXT("0.0.0.0");

	/*UDP Port*/
	UPROPERTY(EditAnywhere, Category = "Connection Settings")
	uint16 UDPPortNumber = 5432;

	/*Multicast Address*/
	UPROPERTY(EditAnywhere, Category = "Connection Settings")
	FString MulticastAddress = TEXT("239.192.1.1");
};