#pragma once

#include "ArriCameraTypes.generated.h"

USTRUCT(BlueprintType)
struct FArriSensorDimensions
{
	GENERATED_BODY()

	float sensorWidth;
	float sensorHeight;
};

UENUM(BlueprintType)
enum class EAlexa35 : uint8
{
	ARRIRAW_OG_46K UMETA(DisplayName = "ARRIRAW OpenGate 4.6K"),
	ARRIRAW_169_46K UMETA(DisplayName = "ARRIRAW 16:9 4.6K"),
	ARRIRAW_169_4K UMETA(DisplayName = "ARRIRAW 16:9 4K"),
	ARRIRAW_21_4K UMETA(DisplayName = "ARRIRAW 2:1 4K"),
	ARRIRAW_169_38K UMETA(DisplayName = "ARRIRAW 16:9 3.8K"),
	ARRIRAW_65_33K UMETA(DisplayName = "ARRIRAW 6:5 3.3K"),
	ARRIRAW_11_3K UMETA(DisplayName = "ARRIRAW 1:1 3K"),
	PRORES_OG_46K UMETA(DisplayName = "ProRes OpenGate 4.6K"),
	PRORES_169_46K UMETA(DisplayName = "ProRes 16:9 4.6K"),
	PRORES_169_4K UMETA(DisplayName = "ProRes 16:9 4K"),
	PRORES_169_UHD UMETA(DisplayName = "ProRes 16:9 UHD"),
	PRORES_169_2K UMETA(DisplayName = "ProRes 16:9 2K"),
	PRORES_169_HD UMETA(DisplayName = "ProRes 16:9 HD"),
	PRORES_21_4K UMETA(DisplayName = "ProRes 2:1 4K"),
	PRORES_65_33K UMETA(DisplayName = "ProRes 6:5 3.3K"),
	PRORES_2391_4K UMETA(DisplayName = "ProRes 2.39:1 4K"),
	PRORES_11_3K UMETA(DisplayName = "ProRes 1:1 3K"),
	PRORES_21_38K UMETA(DisplayName = "ProRes 2:1 3.8K"),
	PRORES_89_27K UMETA(DisplayName = "ProRes 8:9 2.7K"),
	PRORES_169_S16 UMETA(DisplayName = "ProRes 16:9 S16")
};

UENUM(BlueprintType)
enum class EAlexa65 : uint8
{
	ARRIRAW_OG_65K UMETA(DisplayName = "ARRIRAW OpenGate 6.5K"),
	ARRIRAW_169_51K UMETA(DisplayName = "ARRIRAW 16:9 5.1K"),
	ARRIRAW_32_43K UMETA(DisplayName = "ARRIRAW 3:2 4.3K"),
	ARRIRAW_169_4K_UHD UMETA(DisplayName = "ARRIRAW 16:9 4K UHD"),
	ARRIRAW_LF_OG_45K UMETA(DisplayName = "ARRIRAW LF OpenGate 4.5K")
};

UENUM(BlueprintType)
enum class EAlexaLF : uint8
{
	ARRIRAW_LF_169_4K_UHD UMETA(DisplayName = "ARRIRAW LF 16:9 4K UHD"),
	ARRIRAW_LF_2391_Scope_45K UMETA(DisplayNAme = "ARRIRAW LF 2.39:1 Scope 4.5K"),
	ARRIRAW_LF_OG_45K UMETA(DisplayName = "ARRIRAW LF OpenGate 4.5K"),
	PRORES_LF_169_HD UMETA(DisplayName = "ProRes LF 16:9 HD"),
	PRORES_LF_169_2K UMETA(DisplayName = "ProRes LF 16:9 2K"),
	PRORES_LF_169_4K_UHD UMETA(DisplayName = "ProRes LF 16:9 4K UHD"),
	PRORES_LF_2391_Scope_45K UMETA(DisplayName = "ProRes LF 2.39:1 Scope 4.5K"),
	PRORES_LF_OG_45K UMETA(DisplayName = "ProRes LF OpenGate 4.5K")
};

UENUM(BlueprintType)
enum class EAmira : uint8
{
	ARRIRAW_169_28K UMETA(DisplayName = "ARRIRAW 16:9 2.8K"),
	PRORES_169_HD UMETA(DisplayName = "ProRes 16:9 HD"),
	PRORES_169_S16_HD UMETA(DisplayName = "ProRes 16:9 S16 HD"),
	PRORES_169_2K UMETA(DisplayName = "ProRes 16:9 2K"),
	PRORES_169_32K UMETA(DisplayName = "ProRes 16:9 3.2 K"),
	PRORES_169_4K_UHD UMETA(DisplayName = "ProRes 16:9 4K UHD")
};

UENUM(BlueprintType)
enum class EAlexaMiniLF : uint8
{
	ARRIRAW_45K_LF_32_OG_45K UMETA(DisplayName = "ARRIRAW 4.5K LF 3:2 OpenGate - 4.5K"),
	ARRIRAW_45K_LF_2391_45K UMETA(DisplayName = "ARRIRAW 4.5K LF 2.39:1 - 4.5K"),
	ARRIRAW_38K_LF_169_UHD UMETA(DisplayName = "ARRIRAW 3.8K LF 16:9 - UHD"),
	ARRIRAW_28K_LF_11_28K UMETA(DisplayName = "ARRIRAW 2.8K LF 1:1 - 2.8K"),
	ARRIRAW_34K_S35_32_34K UMETA(DisplayName = "ARRIRAW 3.4K S35 3:2 - 3.4K"),
	PRORES_45K_LF_32_OG_45K UMETA(DisplayName = "ProRes 4.5 K LF 3:2 OpenGate - 4.5K"),
	PRORES_45K_LF_2391_45K UMETA(DisplayName = "ProRes 4.5K LF 2.39:1 - 4.5K"),
	PRORES_43K_LF_169_UHD UMETA(DisplayName = "ProRes 4.3K LF 16:9 - UHD"),
	PRORES_43K_LF_169_HD UMETA(DisplayNAme = "ProRes 4.3K LF 16:9 - HD"),
	PRORES_38K_LF_169_UHD UMETA(DisplayName = "ProRes 3.8K LF 16:9 - UHD"),
	PRORES_38K_LF_169_2K UMETA(DisplayName = "ProRes 3.8K LF 16:9 - 2K"),
	PRORES_38K_LF_169_HD UMETA(DisplayName = "ProRes 3.8K LF 16:9 - HD"),
	PRORES_28K_LF_11_28K UMETA(DisplayName = "ProRes 2.8K LF 1:1 - 2.8K"),
	PRORES_32K_S35_169_32K UMETA(DisplayName = "ProRes 3.2K S35 16:9 - 3.2K"),
	PRORES_28K_S35_43_28K UMETA(DisplayName = "ProRes 2.8K S35 4:3 - 2.8K"),
	PRORES_28K_S35_169_HD UMETA(DisplayName = "ProRes 2.8K S35 16:9 - HD")
};

UENUM(BlueprintType)
enum class EAlexaMini : uint8
{
	ARRIRAW_28K_169 UMETA(DisplayName = "ARRIRAW 16:9 - 2.8K"),
	ARRIRAW_OG_34K UMETA(DisplayName = "ARRIRAW OpenGate - 3.4K"),
	ARRIRAW_28K_43 UMETA(DisplayName = "ARRIRAW 4:3 - 2.8K"),
	ARRIRAW_169_HD_ANA UMETA(DisplayNAme = "ARRIRAW 16:9 HD Ana."),
	ARRIRAW_2391_2K_Ana UMETA(DisplayName = "ARRIRAW 2.39:1 2K Ana."),
	PRORES_169_HD UMETA(DisplayName = "ProRes 16:9 HD"),
	PRORES_169_S16_HD UMETA(DisplayName = "ProRes 16:9 S16 HD"),
	PRORES_169_HD_ANA UMETA(DisplayName = "ProRes 16:9 HD Ana."),
	PRORES_169_2K UMETA(DisplayName = "ProRes 16:9 2K"),
	PRORES_2391_2K_ANA UMETA(DisplayName = "ProRes 2.39:1 2K Ana."),
	PRORES_169_32K UMETA(DisplayName = "ProRes 16:9 3.2K"),
	PRORES_169_4K_UHD UMETA(DisplayName = "ProRes 16:9 4K UHD"),
	PRORES_43_28K UMETA(DisplayName = "ProRes 4:3 2.8K")
};

UENUM(BlueprintType)
enum class EArriCameraType : uint8
{
	ALEXA35 UMETA(DisplayName = "ALEXA 35"),
	ALEXALF UMETA(DisplayName = "ALEXA LF"),
	ALEXA65 UMETA(DisplayName = "ALEXA 65"),
	ALEXAMINILF UMETA(DisplayName = "ALEXA Mini LF"),
	ALEXAMINI UMETA(DisplayName = "ALEXA Mini"),
	AMIRA UMETA(DisplayName = "AMIRA")
};
