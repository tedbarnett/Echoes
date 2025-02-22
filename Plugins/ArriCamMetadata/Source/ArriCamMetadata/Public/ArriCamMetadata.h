// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FArriCamMetadataModule : public IModuleInterface
{
public:

	static FArriCamMetadataModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FArriCamMetadataModule>("ArriCamMetadata");
	}
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ArriCamMetadata");
	}
	///** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
