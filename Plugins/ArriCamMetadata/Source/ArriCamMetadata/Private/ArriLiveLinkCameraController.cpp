// Fill out your copyright notice in the Description page of Project Settings.


#include "ArriLiveLinkCameraController.h"
#include "Roles/LiveLinkCameraTypes.h"
#include "CineCameraComponent.h"

#include "Runtime/Launch/Resources/Version.h"

UArriLiveLinkCameraController::UArriLiveLinkCameraController() 
{

}

#if WITH_EDITOR
void UArriLiveLinkCameraController::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	ULiveLinkCameraController::PostEditChangeProperty(PropertyChangedEvent);
	const FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;


#if ENGINE_MAJOR_VERSION == 4 
	if (UCameraComponent* CameraComponent = Cast<UCameraComponent>(AttachedComponent))
#elif ENGINE_MAJOR_VERSION == 5
#if ENGINE_MINOR_VERSION == 0
	if (UCameraComponent* CameraComponent = Cast<UCameraComponent>(AttachedComponent))
#else
	if (UCameraComponent* CameraComponent = Cast<UCameraComponent>(GetAttachedComponent()))
#endif
#endif
	{
		if (UCineCameraComponent* CineCameraComponent = Cast<UCineCameraComponent>(CameraComponent))
		{
			if (setFilmback)
			{
				FArriSensorDimensions SensorSize;

				if (ArriCamera == EArriCameraType::ALEXA35)
				{
					SensorSize = UArriCamMetadataBPLibrary::getAlexa35SensorSize(Alexa35Preset);

					CineCameraComponent->Filmback.SensorWidth = SensorSize.sensorWidth;
					CineCameraComponent->Filmback.SensorHeight = SensorSize.sensorHeight;
				}
				if (ArriCamera == EArriCameraType::ALEXAMINILF)
				{
					SensorSize = UArriCamMetadataBPLibrary::getAlexaMiniLFSensorSize(AlexaMiniLFPreset);

					CineCameraComponent->Filmback.SensorWidth = SensorSize.sensorWidth;
					CineCameraComponent->Filmback.SensorHeight = SensorSize.sensorHeight;
				}
				if (ArriCamera == EArriCameraType::ALEXA65)
				{
					SensorSize = UArriCamMetadataBPLibrary::getAlexa65SensorSize(Alexa65Preset);

					CineCameraComponent->Filmback.SensorWidth = SensorSize.sensorWidth;
					CineCameraComponent->Filmback.SensorHeight = SensorSize.sensorHeight;
				}
				else if (ArriCamera == EArriCameraType::ALEXALF)
				{
					SensorSize = UArriCamMetadataBPLibrary::getAlexaLFSensorSize(AlexaLFPreset);

					CineCameraComponent->Filmback.SensorWidth = SensorSize.sensorWidth;
					CineCameraComponent->Filmback.SensorHeight = SensorSize.sensorHeight;
				}
				else if (ArriCamera == EArriCameraType::AMIRA)
				{
					SensorSize = UArriCamMetadataBPLibrary::getAmiraSensorSize(AmiraPreset);

					CineCameraComponent->Filmback.SensorWidth = SensorSize.sensorWidth;
					CineCameraComponent->Filmback.SensorHeight = SensorSize.sensorHeight;
				}
				else if(ArriCamera == EArriCameraType::ALEXAMINI)
				{
					SensorSize = UArriCamMetadataBPLibrary::getAlexaMiniSensorSize(AlexaMiniPreset);

					CineCameraComponent->Filmback.SensorWidth = SensorSize.sensorWidth;
					CineCameraComponent->Filmback.SensorHeight = SensorSize.sensorHeight;
				}
			}
		}
	}
}
#endif

void UArriLiveLinkCameraController::Tick(float DeltaTime, const FLiveLinkSubjectFrameData& SubjectData)
{
	ULiveLinkCameraController::Tick(DeltaTime, SubjectData);

	const FLiveLinkCameraStaticData* StaticData = SubjectData.StaticData.Cast<FLiveLinkCameraStaticData>();
	const FLiveLinkCameraFrameData* FrameData = SubjectData.FrameData.Cast<FLiveLinkCameraFrameData>();
#if ENGINE_MAJOR_VERSION == 4 
	if (UCameraComponent* CameraComponent = Cast<UCameraComponent>(AttachedComponent))
#elif ENGINE_MAJOR_VERSION == 5
#if ENGINE_MINOR_VERSION == 0
	if (UCameraComponent* CameraComponent = Cast<UCameraComponent>(AttachedComponent))
#else
	if (UCameraComponent* CameraComponent = Cast<UCameraComponent>(GetAttachedComponent()))
#endif
#endif
	{
		if (UCineCameraComponent* CineCameraComponent = Cast<UCineCameraComponent>(CameraComponent))
		{
			CineCameraComponent->FocusSettings.ManualFocusDistance = FrameData->FocusDistance * depthOfFieldMultiplier;
			if (overrideIris == true)
			{
				CineCameraComponent->CurrentAperture = newIris;
			}			
		}
	}
}
