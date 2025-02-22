// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArriCamMetadataBPLibrary.h"
#include "Common/UdpSocketBuilder.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

#define RECV_BUFFER_SIZE 1024 * 1024

UArriDevice* UArriCamMetadataBPLibrary::initDevice(const UDPMode Mode, const FString IPAddress, const int UDPPortNumber, const FString MulticastAddress)
{
	bool createSocket = true;

	UArriDevice* ArriDevice = NewObject<UArriDevice>();

	ArriDevice->mConnectionSettings.Mode = Mode;
	ArriDevice->mConnectionSettings.IPAddress = IPAddress;
	ArriDevice->mConnectionSettings.UDPPortNumber = UDPPortNumber;
	ArriDevice->mConnectionSettings.MulticastAddress = MulticastAddress;

	ArriDevice->mSocket = UArriDevice::createSocket(ArriDevice->mConnectionSettings, ArriDevice->mSocket);
	ArriDevice->Start();

	return ArriDevice;
}

FArriSensorDimensions UArriCamMetadataBPLibrary::getAlexa35SensorSize(EAlexa35 Alexa35Preset)
{
	FArriSensorDimensions SensorDimensions;

	if (Alexa35Preset == EAlexa35::ARRIRAW_OG_46K || Alexa35Preset == EAlexa35::PRORES_OG_46K)
	{
		SensorDimensions.sensorWidth = 28.0;
		SensorDimensions.sensorHeight = 19.2;
	}
	else if (Alexa35Preset == EAlexa35::ARRIRAW_169_46K || Alexa35Preset == EAlexa35::PRORES_169_46K)
	{
		SensorDimensions.sensorWidth = 28.0;
		SensorDimensions.sensorHeight = 15.7;
	}
	else if (Alexa35Preset == EAlexa35::ARRIRAW_169_4K || Alexa35Preset == EAlexa35::PRORES_169_4K || 
			 Alexa35Preset == EAlexa35::PRORES_169_UHD || Alexa35Preset == EAlexa35::PRORES_169_2K || 
			 Alexa35Preset == EAlexa35::PRORES_169_HD)
	{
		SensorDimensions.sensorWidth = 24.9;
		SensorDimensions.sensorHeight = 14.0;
	}
	else if (Alexa35Preset == EAlexa35::ARRIRAW_21_4K || Alexa35Preset == EAlexa35::PRORES_21_4K)
	{
		SensorDimensions.sensorWidth = 24.9;
		SensorDimensions.sensorHeight = 12.4;
	}
	else if (Alexa35Preset == EAlexa35::ARRIRAW_169_38K)
	{
		SensorDimensions.sensorWidth = 23.3;
		SensorDimensions.sensorHeight = 13.1;
	}
	else if (Alexa35Preset == EAlexa35::ARRIRAW_65_33K || Alexa35Preset == EAlexa35::PRORES_65_33K || 
			 Alexa35Preset == EAlexa35::PRORES_2391_4K)
	{
		SensorDimensions.sensorWidth = 20.2;
		SensorDimensions.sensorHeight = 16.9;
	}
	else if (Alexa35Preset == EAlexa35::ARRIRAW_11_3K || Alexa35Preset == EAlexa35::PRORES_11_3K || 
			 Alexa35Preset == EAlexa35::PRORES_21_38K)
	{
		SensorDimensions.sensorWidth = 18.7;
		SensorDimensions.sensorHeight = 18.7;
	}
	else if (Alexa35Preset == EAlexa35::PRORES_89_27K)
	{
		SensorDimensions.sensorWidth = 16.7;
		SensorDimensions.sensorHeight = 18.8;
	}
	else if (Alexa35Preset == EAlexa35::PRORES_169_S16)
	{
		SensorDimensions.sensorWidth = 12.4;
		SensorDimensions.sensorHeight = 7.0;
	}
	else
	{
		SensorDimensions.sensorWidth = 16;
		SensorDimensions.sensorHeight = 9;
	}
	return SensorDimensions;
}

FArriSensorDimensions UArriCamMetadataBPLibrary::getAlexaMiniLFSensorSize(EAlexaMiniLF AlexaMiniLFPreset)
{
	FArriSensorDimensions SensorDimensions;

	if (AlexaMiniLFPreset == EAlexaMiniLF::ARRIRAW_45K_LF_32_OG_45K || AlexaMiniLFPreset == EAlexaMiniLF::PRORES_45K_LF_32_OG_45K)
	{
		SensorDimensions.sensorWidth = 36.7;
		SensorDimensions.sensorHeight = 25.54;
	}
	else if (AlexaMiniLFPreset == EAlexaMiniLF::ARRIRAW_45K_LF_2391_45K || AlexaMiniLFPreset == EAlexaMiniLF::PRORES_45K_LF_2391_45K)
	{
		SensorDimensions.sensorWidth = 36.7;
		SensorDimensions.sensorHeight = 15.31;
	}
	else if (AlexaMiniLFPreset == EAlexaMiniLF::ARRIRAW_38K_LF_169_UHD || AlexaMiniLFPreset == EAlexaMiniLF::PRORES_38K_LF_169_UHD ||
		AlexaMiniLFPreset == EAlexaMiniLF::PRORES_38K_LF_169_2K || AlexaMiniLFPreset == EAlexaMiniLF::PRORES_38K_LF_169_HD)
	{
		SensorDimensions.sensorWidth = 31.68;
		SensorDimensions.sensorHeight = 17.82;
	}
	else if (AlexaMiniLFPreset == EAlexaMiniLF::ARRIRAW_28K_LF_11_28K || AlexaMiniLFPreset == EAlexaMiniLF::PRORES_28K_LF_11_28K)
	{
		SensorDimensions.sensorWidth = 23.76;
		SensorDimensions.sensorHeight = 23.76;
	}
	else if (AlexaMiniLFPreset == EAlexaMiniLF::ARRIRAW_34K_S35_32_34K)
	{
		SensorDimensions.sensorWidth = 28.25;
		SensorDimensions.sensorHeight = 18.17;
	}
	else if (AlexaMiniLFPreset == EAlexaMiniLF::PRORES_43K_LF_169_UHD || AlexaMiniLFPreset == EAlexaMiniLF::PRORES_43K_LF_169_HD)
	{
		SensorDimensions.sensorWidth = 35.64;
		SensorDimensions.sensorHeight = 20.05;
	}
	else if (AlexaMiniLFPreset == EAlexaMiniLF::PRORES_32K_S35_169_32K)
	{
		SensorDimensions.sensorWidth = 26.40;
		SensorDimensions.sensorHeight = 14.85;
	}
	else if (AlexaMiniLFPreset == EAlexaMiniLF::PRORES_28K_S35_43_28K)
	{
		SensorDimensions.sensorWidth = 23.76;
		SensorDimensions.sensorHeight = 17.82;
	}
	else if (AlexaMiniLFPreset == EAlexaMiniLF::PRORES_28K_S35_169_HD)
	{
		SensorDimensions.sensorWidth = 23.76;
		SensorDimensions.sensorHeight = 13.37;
	}
	else
	{
		SensorDimensions.sensorWidth = 16;
		SensorDimensions.sensorHeight = 9;
	}
	return SensorDimensions;
}

FArriSensorDimensions UArriCamMetadataBPLibrary::getAlexaMiniSensorSize(EAlexaMini AlexaMiniPreset)
{
	FArriSensorDimensions SensorDimensions;

	if (AlexaMiniPreset == EAlexaMini::ARRIRAW_OG_34K)
	{
		SensorDimensions.sensorWidth = 28.25;
		SensorDimensions.sensorHeight = 18.17;
	}
	else if (AlexaMiniPreset == EAlexaMini::PRORES_169_32K || AlexaMiniPreset == EAlexaMini::PRORES_169_4K_UHD)
	{
		SensorDimensions.sensorWidth = 26.40;
		SensorDimensions.sensorHeight = 14.85;
	}
	else if (AlexaMiniPreset == EAlexaMini::ARRIRAW_28K_43 || AlexaMiniPreset == EAlexaMini::PRORES_43_28K)
	{
		SensorDimensions.sensorWidth = 23.76;
		SensorDimensions.sensorHeight = 17.82;
	}
	else if (AlexaMiniPreset == EAlexaMini::ARRIRAW_28K_169)
	{
		SensorDimensions.sensorWidth = 27.26;
		SensorDimensions.sensorHeight = 13.37;
	}
	else if (AlexaMiniPreset == EAlexaMini::ARRIRAW_169_HD_ANA)
	{
		SensorDimensions.sensorWidth = 15.84;
		SensorDimensions.sensorHeight = 17.82;
	}
	else if (AlexaMiniPreset == EAlexaMini::PRORES_169_2K)
	{
		SensorDimensions.sensorWidth = 23.66;
		SensorDimensions.sensorHeight = 13.3;
	}
	else if (AlexaMiniPreset == EAlexaMini::PRORES_2391_2K_ANA)
	{
		SensorDimensions.sensorWidth = 21.12;
		SensorDimensions.sensorHeight = 17.7;
	}
	else
	{
		SensorDimensions.sensorWidth = 16;
		SensorDimensions.sensorHeight = 9;
	}
	return SensorDimensions;
}

FArriSensorDimensions UArriCamMetadataBPLibrary::getAlexaLFSensorSize(EAlexaLF AlexaLFPreset)
{
	FArriSensorDimensions SensorDimensions;

	if (AlexaLFPreset == EAlexaLF::ARRIRAW_LF_OG_45K || AlexaLFPreset == EAlexaLF::PRORES_LF_OG_45K)
	{
		SensorDimensions.sensorWidth = 36.7;
		SensorDimensions.sensorHeight = 25.54;
	}
	else if (AlexaLFPreset == EAlexaLF::ARRIRAW_LF_2391_Scope_45K || AlexaLFPreset == EAlexaLF::PRORES_LF_2391_Scope_45K)
	{
		SensorDimensions.sensorWidth = 36.7;
		SensorDimensions.sensorHeight = 15.31;
	}
	else if (AlexaLFPreset == EAlexaLF::ARRIRAW_LF_169_4K_UHD || AlexaLFPreset == EAlexaLF::PRORES_LF_169_HD ||
			 AlexaLFPreset == EAlexaLF::PRORES_LF_169_2K || AlexaLFPreset == EAlexaLF::PRORES_LF_169_4K_UHD)
	{
		SensorDimensions.sensorWidth = 31.68;
		SensorDimensions.sensorHeight = 17.82;
	}
	else
	{
		SensorDimensions.sensorWidth = 16;
		SensorDimensions.sensorHeight = 9;
	}
	return SensorDimensions;
}

FArriSensorDimensions UArriCamMetadataBPLibrary::getAlexa65SensorSize(EAlexa65 Alexa65Preset)
{
	FArriSensorDimensions SensorDimensions;

	if (Alexa65Preset == EAlexa65::ARRIRAW_LF_OG_45K)
	{
		SensorDimensions.sensorWidth = 36.7;
		SensorDimensions.sensorHeight = 25.54;
	}
	else if (Alexa65Preset == EAlexa65::ARRIRAW_169_4K_UHD)
	{
		SensorDimensions.sensorWidth = 31.68;
		SensorDimensions.sensorHeight = 17.82;
	}
	else if (Alexa65Preset == EAlexa65::ARRIRAW_OG_65K)
	{
		SensorDimensions.sensorWidth = 54.12;
		SensorDimensions.sensorHeight = 25.58;
	}
	else if (Alexa65Preset == EAlexa65::ARRIRAW_169_51K)
	{
		SensorDimensions.sensorWidth = 42.24;
		SensorDimensions.sensorHeight = 23.76;
	}
	else if (Alexa65Preset == EAlexa65::ARRIRAW_32_43K)
	{
		SensorDimensions.sensorWidth = 35.64;
		SensorDimensions.sensorHeight = 23.76;
	}
	else
	{
		SensorDimensions.sensorWidth = 16;
		SensorDimensions.sensorHeight = 9;
	}
	return SensorDimensions;
}

FArriSensorDimensions UArriCamMetadataBPLibrary::getAmiraSensorSize(EAmira AmiraPreset)
{
	FArriSensorDimensions SensorDimensions;

	if (AmiraPreset == EAmira::PRORES_169_32K || AmiraPreset == EAmira::PRORES_169_4K_UHD)
	{
		SensorDimensions.sensorWidth = 26.40;
		SensorDimensions.sensorHeight = 14.85;
	}
	else if (AmiraPreset == EAmira::ARRIRAW_169_28K || AmiraPreset == EAmira::PRORES_169_HD)
	{
		SensorDimensions.sensorWidth = 23.76;
		SensorDimensions.sensorHeight = 13.37;
	}
	else if (AmiraPreset == EAmira::PRORES_169_2K)
	{
		SensorDimensions.sensorWidth = 23.66;
		SensorDimensions.sensorHeight = 13.3;
	}
	else if (AmiraPreset == EAmira::PRORES_169_S16_HD)
	{
		SensorDimensions.sensorWidth = 13.2;
		SensorDimensions.sensorHeight = 7.43;
	}
	else
	{
		SensorDimensions.sensorWidth = 16;
		SensorDimensions.sensorHeight = 9;
	}
	return SensorDimensions;
}