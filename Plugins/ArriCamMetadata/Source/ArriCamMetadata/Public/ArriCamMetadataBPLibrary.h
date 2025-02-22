// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ArriMetadata.h"
#include "ArriDevice.h"
#include "ArriLiveLinkConnectionSettings.h"
#include "HAL/Runnable.h"
#include "ArriCameraTypes.h"

#include "ArriCamMetadataBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/

UCLASS()
class UArriCamMetadataBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Arri")
	static UArriDevice* initDevice(const UDPMode Mode = UDPMode::MULTI, const FString IPAddress = TEXT("0.0.0.0"), const int UDPPortNumber = 5432, const FString MulticastAddress = TEXT("239.192.1.1"));

public:
	UFUNCTION(BlueprintPure, Category = "Arri")
	static FArriSensorDimensions getAlexa35SensorSize(const EAlexa35 Alexa35Preset = EAlexa35::ARRIRAW_169_4K);
	UFUNCTION(BlueprintPure, Category = "Arri")
	static FArriSensorDimensions getAlexaMiniLFSensorSize(const EAlexaMiniLF AlexaMiniLFPreset = EAlexaMiniLF::ARRIRAW_38K_LF_169_UHD);
	UFUNCTION(BlueprintPure, Category = "Arri")
	static FArriSensorDimensions getAlexaMiniSensorSize(const EAlexaMini AlexaMiniPreset = EAlexaMini::ARRIRAW_28K_169);
	UFUNCTION(BlueprintPure, Category = "Arri")
	static FArriSensorDimensions getAlexa65SensorSize(const EAlexa65 Alexa65Preset = EAlexa65::ARRIRAW_OG_65K);
	UFUNCTION(BlueprintPure, Category = "Arri")
	static FArriSensorDimensions getAlexaLFSensorSize(const EAlexaLF AlexaLFPreset = EAlexaLF::ARRIRAW_LF_169_4K_UHD);
	UFUNCTION(BlueprintPure, Category = "Arri")
	static FArriSensorDimensions getAmiraSensorSize(const EAmira AmiraPreset = EAmira::ARRIRAW_169_28K);
};
