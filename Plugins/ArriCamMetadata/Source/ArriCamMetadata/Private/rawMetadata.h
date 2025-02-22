#pragma once
#include "rawMetadata.generated.h"


USTRUCT()
struct FCameraDevice
{
	GENERATED_BODY()

	//only UMC4 relevant
	UPROPERTY()
	FString model;
	//only UMC4 relevant
	UPROPERTY()
	FString umc4SerialNumber;
	//only UMC4 relevant
	UPROPERTY()
	FString umc4SoftwareVersion;
	UPROPERTY()
	FString cameraModel;
	UPROPERTY()
	FString cameraSerialNumber;
	UPROPERTY()
	FString cameraSoftwarePackageName;
	UPROPERTY()
	FString cameraSoftwarePackageVersion;
	UPROPERTY()
	FString cameraVariant;
};

USTRUCT()
struct FLensConverter
{
	GENERATED_BODY()

	UPROPERTY()
	double converterFocalLenMultiplier;
	UPROPERTY()
	double converterLightLossFactor;
	UPROPERTY()
	FString converterModel;
	UPROPERTY()
	int64 converterPhysicalLength;
	UPROPERTY()
	FString converterSerialNumber;
};

USTRUCT()
struct FFilter
{
	GENERATED_BODY()

	UPROPERTY()
	double ndFilterDensity;
};


USTRUCT()
struct FLensFocusDistanceLimitsImperial
{
	GENERATED_BODY()

	UPROPERTY()
	int64 max;
	UPROPERTY()
	int64 min;
};

USTRUCT()
struct FLensFocusDistanceLimitsMetric
{
	GENERATED_BODY()

	UPROPERTY()
	int64 max;
	UPROPERTY()
	int64 min;
};

USTRUCT()
struct FLensLinearIrisLimits
{
	GENERATED_BODY()

	UPROPERTY()
	int64 max;
	UPROPERTY()
	int64 min;
};

USTRUCT()
struct FLensFocalLengthLimits
{
	GENERATED_BODY()

	UPROPERTY()
	int64 max;
	UPROPERTY()
	int64 min;
};

USTRUCT()
struct FLensMotorRawEncoderFocalLengthLimits
{
	GENERATED_BODY()

	UPROPERTY()
	int64 max;
	UPROPERTY()
	int64 min;
};

USTRUCT()
struct FLensMotorRawEncoderFocusLimits
{
	GENERATED_BODY()

	UPROPERTY()
	int64 max;
	UPROPERTY()
	int64 min;
};

USTRUCT()
struct FLensMotorRawEncoderIrisLimits
{
	GENERATED_BODY()

	UPROPERTY()
	int64 max;
	UPROPERTY()
	int64 min;
};

USTRUCT()
struct FLensScaleRawEncoderFocalLengthLimits
{
	GENERATED_BODY()

	UPROPERTY()
	int64 max;
	UPROPERTY()
	int64 min;
};

USTRUCT()
struct FLensScaleRawEncoderFocusLimits
{
	GENERATED_BODY()

	UPROPERTY()
	int64 max;
	UPROPERTY()
	int64 min;
};

USTRUCT()
struct FLensScaleRawEncoderIrisLimits
{
	GENERATED_BODY()

	UPROPERTY()
	int64 max;
	UPROPERTY()
	int64 min;
};


USTRUCT()
struct FLensDevice
{
	GENERATED_BODY()

	UPROPERTY()
	float circleOfConfusion;
	UPROPERTY()
	FLensFocusDistanceLimitsImperial lensLimitsFocusImperial;
	UPROPERTY()
	FLensFocusDistanceLimitsMetric lensLimitsFocusMetric;
	UPROPERTY()
	FLensLinearIrisLimits lensLimitsIris;
	UPROPERTY()
	FLensFocalLengthLimits lensLimitsZoom;
	UPROPERTY()
	FString lensModel;
	UPROPERTY()
	FLensMotorRawEncoderFocalLengthLimits lensMotorRawEncoderLimitsFL;
	UPROPERTY()
	FLensMotorRawEncoderFocusLimits lensMotorRawEncoderLimitsFocus;
	UPROPERTY()
	FLensMotorRawEncoderIrisLimits lensMotorRawEncoderLimitsIris;
	UPROPERTY()
	FLensScaleRawEncoderFocalLengthLimits lensScaleRawEncoderLimitsFL;
	UPROPERTY()
	FLensScaleRawEncoderFocusLimits lensScaleRawEncoderLimitsFocus;
	UPROPERTY()
	FLensScaleRawEncoderIrisLimits lensScaleRawEncoderLimitsIris;
	UPROPERTY()
	FString lensScaleRawEncoderSource;
	UPROPERTY()
	FString serialNumber;
	UPROPERTY()
	FString lensSqueezeFactor;
};

USTRUCT()
struct FLensState
{
	GENERATED_BODY()

	UPROPERTY()
	int64 lensEffectiveFocalLength;
	UPROPERTY()
	int64 lensEntrancePupilOffset;
	UPROPERTY()
	int64 lensFocalLength;
	UPROPERTY()
	int64 lensFocusDistanceMetric;
	UPROPERTY()
	int64 lensFocusDistanceImperial;
	UPROPERTY()
	int64 lensIris;
	UPROPERTY()
	int64 lensMotorRawEncoderValueFl;
	UPROPERTY()
	int64 lensMotorRawEncoderValueFocus;
	UPROPERTY()
	int64 lensMotorRawEncoderValueIris;
	UPROPERTY()
	bool lensOisActive;
	UPROPERTY()
	int64 lensScaleRawEncoderValueFl;
	UPROPERTY()
	int64 lensScaleRawEncoderValueFocus;
	UPROPERTY()
	int64 lensScaleRawEncoderValueIris;
};


USTRUCT()
struct FLens
{
	GENERATED_BODY()

	UPROPERTY()
	FLensDevice lensDevice;
	UPROPERTY()
	FLensState lensState;
};

USTRUCT()
struct FOptic
{
	GENERATED_BODY()

	UPROPERTY()
	FLensConverter converter;
	UPROPERTY()
	FFilter filter;
	UPROPERTY()
	FLens lens;
};

USTRUCT()
struct FOrientation
{
	GENERATED_BODY()

	UPROPERTY()
	double pan;
	UPROPERTY()
	double roll;
	UPROPERTY()
	double tilt;
};

USTRUCT()
struct FPositional
{
	GENERATED_BODY()

	UPROPERTY()
	FOrientation orientation;
};

USTRUCT()
struct FWhiteBalance
{
	GENERATED_BODY()

	UPROPERTY()
	int64 colorTemperature;
	UPROPERTY()
	double whiteBalanceTint;
};

USTRUCT()
struct FProcessing
{
	GENERATED_BODY()

	UPROPERTY()
	FWhiteBalance whiteBalance;
};

USTRUCT()
struct FSlateInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FString cameraIndex;
	UPROPERTY()
	FString clipName;
	UPROPERTY()
	int64 clipNumber;
};

USTRUCT()
struct FClip
{
	GENERATED_BODY()

	UPROPERTY()
	FSlateInfo slateInfo;
};


USTRUCT()
struct FRecordingMedium
{
	GENERATED_BODY()

	//only UMC4 relevant
	UPROPERTY()
	int16 remainingCapacity;
	UPROPERTY()
	int64 mediumCapacity;

};

USTRUCT()
struct FProjectRate
{
	GENERATED_BODY()

	UPROPERTY()
	bool dropframe;
	UPROPERTY()
	FString timebase;
};

USTRUCT()
struct FRecording
{
	GENERATED_BODY()

	UPROPERTY()
	int32 acquisitionTimestamp;
	UPROPERTY()
	FClip clip;
	UPROPERTY()
	FRecordingMedium medium;
	UPROPERTY()
	FProjectRate projectRate;
};

USTRUCT()
struct FSensorState
{
	GENERATED_BODY()

	UPROPERTY()
	int32 averagedFrames;
	UPROPERTY()
	int64 exposureIndex;
	//only UMC4 relevant
	UPROPERTY()
	double shutterAngle;
	UPROPERTY()
	FString exposureTime;
	UPROPERTY()
	FString sensorSampleRate;
};

USTRUCT()
struct FSensor
{
	GENERATED_BODY()

	UPROPERTY()
	FSensorState sensorState;
};

USTRUCT()
struct FArriCameraStateRaw
{
	GENERATED_BODY()

	UPROPERTY()
	FString health;
	UPROPERTY()
	FString majorState;
};

USTRUCT()
struct FArriCamera
{
	GENERATED_BODY()

	UPROPERTY()
	FCameraDevice device;
	UPROPERTY()
	FOptic optic;
	UPROPERTY()
	FPositional positional;
	UPROPERTY()
	FProcessing processing;
	UPROPERTY()
	FRecording recording;
	UPROPERTY()
	FSensor sensor;
	UPROPERTY()
	FArriCameraStateRaw state;
};

USTRUCT()
struct FArriCameraRawMatadata
{
	GENERATED_BODY()

	UPROPERTY()
	FArriCamera camera;
};