// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LiveLinkCameraController.h"
#include "ArriCameraTypes.h"
#include "ArriCamMetadataBPLibrary.h"


#include "ArriLiveLinkCameraController.generated.h"

/**
Arri Live Link Controller enables you to manipulate the streamed Metadata so you can easily match your unreal content to your real camera image.
 */
UCLASS()
class ARRICAMMETADATA_API UArriLiveLinkCameraController : public ULiveLinkCameraController
{
	GENERATED_BODY()
public:
	/** Manipulate the Depth of field value to move the focus plane further away (e.g 1.5) or closer (e.g 0.5) to the real camera. */
	UPROPERTY(EditAnywhere, Category = "Arri")
	float depthOfFieldMultiplier = 1.0;
	
	/** Check if you would like to override the Iris of the virtual camera, instead of using the streamed value */
	UPROPERTY(EditAnywhere, Category = "Arri")
	bool overrideIris = false;

	/** Input new Iris value */
	UPROPERTY(EditAnywhere, Category = "Arri", meta = (EditCondition = "overrideIris"))
	float newIris = 2.8;

	/** Set Filmback from Preset */
	UPROPERTY(EditAnywhere, Category = "Arri")
	bool setFilmback = false;

	/** Choose camera*/
	UPROPERTY(EditAnywhere, Category = "Arri", meta = (EditCondition = "setFilmback"))
	EArriCameraType ArriCamera = EArriCameraType::ALEXAMINILF;

	/** Choose Preset for Alexa35*/
	UPROPERTY(EditAnywhere, Category = "Arri", meta = (EditCondition = "setFilmback && ArriCamera == EArriCameraType::ALEXA35"))
	EAlexa35 Alexa35Preset = EAlexa35::ARRIRAW_169_4K;

	/** Choose Preset for Alexa MiniLF*/
	UPROPERTY(EditAnywhere, Category = "Arri", meta = (EditCondition = "setFilmback && ArriCamera == EArriCameraType::ALEXAMINILF"))
	EAlexaMiniLF AlexaMiniLFPreset = EAlexaMiniLF::ARRIRAW_38K_LF_169_UHD;

	/** Choose Preset for Alexa Mini*/
	UPROPERTY(EditAnywhere, Category = "Arri", meta = (EditCondition = "setFilmback && ArriCamera == EArriCameraType::ALEXAMINI"))
	EAlexaMini AlexaMiniPreset = EAlexaMini::ARRIRAW_28K_169;

	/** Choose Preset for Alexa LF*/
	UPROPERTY(EditAnywhere, Category = "Arri", meta = (EditCondition = "setFilmback && ArriCamera == EArriCameraType::ALEXALF"))
	EAlexaLF AlexaLFPreset = EAlexaLF::ARRIRAW_LF_169_4K_UHD;

	/** Choose Preset for Alexa 65*/
	UPROPERTY(EditAnywhere, Category = "Arri", meta = (EditCondition = "setFilmback && ArriCamera == EArriCameraType::AMIRA"))
	EAmira AmiraPreset = EAmira::ARRIRAW_169_28K;

	/** Choose Preset for Amira*/
	UPROPERTY(EditAnywhere, Category = "Arri", meta = (EditCondition = "setFilmback && ArriCamera == EArriCameraType::ALEXA65"))
	EAlexa65 Alexa65Preset = EAlexa65::ARRIRAW_169_4K_UHD;
	UArriLiveLinkCameraController();
	virtual void Tick(float DeltaTime, const FLiveLinkSubjectFrameData& SubjectData) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};