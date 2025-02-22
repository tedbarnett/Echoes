#pragma once
#include "Misc/Timecode.h"
#include "ArriMetadata.generated.h"

UENUM()
enum class NDFilterType : uint8
{
	ND_OFF UMETA(DisplayName = "ND filter off"),
	ND_ON UMETA(DisplayName = "ND filter on"),
	ND_UNKNOWN UMETA(DisplayName = "ND filter unknown")
};

UENUM()
enum class FArriCameraHealthState : uint8
{
	GOOD UMETA(DisplayName = "Camera health state Good"),
	ERROR UMETA(DisplayName = "Camera health state Error"),
	WARNING UMETA(DisplayName = "Camera health state Warning"),
	UNKNOWN UMETA(DisplayName = "Camera health state Warning")
};

UENUM()
enum class FArriCameraMajorState : uint8
{
	CS_IDLE UMETA(DisplayName = "Camera system major state idle"),
	CS_STANDBY UMETA(DisplayName = "Camera system major state standby"),
	CS_REC_START UMETA(DisplayName = "Camera system major state recording start"),
	CS_RECORDING UMETA(DisplayName = "Camera system major state recording"),
	CS_REC_STOP UMETA(DisplayName = "Camera system major state recording stop"),
	CS_PREREC_START UMETA(DisplayName = "Camera system major state prerecording start"),
	CS_PRE_RECORDING UMETA(DisplayName = "Camera system major state prerecording"),
	CS_PREREC_STOP UMETA(DisplayName = "Camera system major state prerecording stop"),
	CS_PLAYBACK UMETA(DisplayName = "Camera system major state playback"),
	CS_ERASE UMETA(DisplayName = "Camera system major state erase"),
	CS_DELETE UMETA(DisplayName = "Camera system major state delete"),
	CS_INITIALIZE UMETA(DisplayName = "Camera system major state initialize"),
	CS_SHUTDOWN UMETA(DisplayName = "Camera system major state shutdown"),
	CS_UPDATE UMETA(DisplayName = "Camera system major state update")
};

USTRUCT(BlueprintType)
struct FArriTimecode
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FTimecode timecode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	bool dropFrame;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FFrameRate timebase;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FFrameTime aquisitionTimestamp; //aka FrameNumber from timecode
};

USTRUCT(BlueprintType)
struct FArriCameraState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FArriCameraHealthState health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FArriCameraMajorState majorState;
};

USTRUCT(BlueprintType)
struct FArriLensConverter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	float converterFocalLenMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	float converterLightLossFactor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FString converterModel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int converterPhysicalLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FString converterSerialNumber;
};

USTRUCT(BlueprintType)
struct FLensLimitsFocusImperial
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int min;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int max;
};

USTRUCT(BlueprintType)
struct FLensLimitsFocusMetric
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int min;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int max;
};

USTRUCT(BlueprintType)
struct FLensLimitsIris
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int min;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int max;
};

USTRUCT(BlueprintType)
struct FLensLimitsZoom
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int min;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int max;
};

USTRUCT(BlueprintType)
struct FMotorRawEncoderLimitsFocus
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int min;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int max;
};

USTRUCT(BlueprintType)
struct FMotorRawEncoderLimitsFocalLength
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int min;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int max;
};

USTRUCT(BlueprintType)
struct FMotorRawEncoderLimitsIris
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int min;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int max;
};


USTRUCT(BlueprintType)
struct FArriMetadata
{
	GENERATED_BODY()

	//camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FArriCameraState cameraState;

	//UMC4 specific Metadata
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FString umcSerialNumber;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FString umcSoftwareVersion;

	//camera -> device
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FString cameraModel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FString cameraSerial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FString cameraSoftwarePackageName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FString cameraSoftwarePackageVersion;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FString cameraVariant;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FString lensModel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FString lensSerialNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FArriTimecode timecode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int effectiveFocalLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	float focusDistanceImperial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	float focusDistanceMetric;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	float aperture;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	float focalLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	float lensSqueezeFactor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int entrancePupilOffset;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	float circleOfConfusion;

	FString lensScaleRawEncoderSource;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FLensLimitsFocusImperial lensLimitsFocusImperial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FLensLimitsFocusMetric lensLimitsFocusMetric;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FLensLimitsIris lensLimitsIris;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FLensLimitsZoom lensLimitsZoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int lensMotorRawEncoderValueFocalLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int lensMotorRawEncoderValueFocus;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int lensMotorRawEncoderValueIris;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int lensScaleRawEncoderValueFocalLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int lensScaleRawEncoderValueFocus;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int lensScaleRawEncoderValueIris;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FMotorRawEncoderLimitsFocus motorRawEncoderLimitsFocus;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FMotorRawEncoderLimitsFocalLength motorRawEncoderLimitsFocalLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FMotorRawEncoderLimitsIris motorRawEncoderLimitsIris;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FArriLensConverter lensConverter;

	//camera -> optic -> filter
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	float ndFilterDensity;


	//camera -> processing -> whitebalance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int colorTemperature;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	float whiteBalanceTint;

	//camera -> recording -> (clip/medium/ProjectRate) -> (slateInfo) 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FString cameraIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FString clipName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int clipNumber;

	//camera -> sensor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int averagedFrames;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int exposureIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	float exposureTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	float shutterAngle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	int mediumCapacity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	FFrameRate sensorSampleRate;

	//camera -> positional -> orientation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	float pan;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	float roll;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArriCamMetadata")
	float tilt;
};


USTRUCT(BlueprintType)
struct FPluginMetadata
{
	GENERATED_BODY()

	UPROPERTY()
	FString versionName = "-- version could not be read --";

};
