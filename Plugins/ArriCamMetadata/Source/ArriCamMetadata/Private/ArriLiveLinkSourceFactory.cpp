#include "ArriLiveLinkSourceFactory.h"
#include "SArriLiveLinkSourceFactory.h"
#include "ArriLiveLinkDevice.h"

#include "ArriDevice.h"

#include "Features/IModularFeatures.h"
#include "ILiveLinkClient.h"
#include "ArriCamMetadata.h"

#define LOCTEXT_NAMESPACE "ArriLiveLinkSourceFactory"

FText UArriLiveLinkSourceFactory::GetSourceDisplayName() const
{
	return LOCTEXT("ArriLiveLinkSourceName", "Arri Live Link Source");
}

FText UArriLiveLinkSourceFactory::GetSourceTooltip() const
{
	return LOCTEXT("ArriLiveLinkSourceTooltip", "Arri UDP Metadata Source");
}


TSharedPtr<SWidget> UArriLiveLinkSourceFactory::BuildCreationPanel(FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const
{
	return SNew(SArriLiveLinkSourceFactory)
		.OnConnectionSettingsAccepted(FOnArriLiveLinkConnectionSettingsAccepted::CreateUObject(this, &UArriLiveLinkSourceFactory::CreateSourceFromSettings, OnLiveLinkSourceCreated));
}

TSharedPtr<ILiveLinkSource> UArriLiveLinkSourceFactory::CreateSource(const FString& ConnectionString) const
{
	FArriLiveLinkConnectionSettings ConnectionSettings;
	if (!ConnectionString.IsEmpty())
	{
		FArriLiveLinkConnectionSettings::StaticStruct()->ImportText(*ConnectionString, &ConnectionSettings, nullptr, PPF_None, GLog, TEXT("UArriLiveLinkSourceFactory"));
	}
	return MakeShared<FArriLiveLinkDevice>(ConnectionSettings);
}

void UArriLiveLinkSourceFactory::CreateSourceFromSettings(FArriLiveLinkConnectionSettings InConnectionSettings, FOnLiveLinkSourceCreated OnSourceCreated) const
{
	FString ConnectionString;
	FArriLiveLinkConnectionSettings::StaticStruct()->ExportText(ConnectionString, &InConnectionSettings, nullptr, nullptr, PPF_None, nullptr);

	TSharedPtr<FArriLiveLinkDevice> SharedPtr = MakeShared<FArriLiveLinkDevice>(InConnectionSettings);
	OnSourceCreated.ExecuteIfBound(SharedPtr, MoveTemp(ConnectionString));
}

#undef LOCTEXT_NAMESPACE