#pragma once
#include "LiveLinkSourceFactory.h"
#include "ArriLiveLinkConnectionSettings.h"
#include "ArriLiveLinkSourceFactory.generated.h"

UCLASS()
class UArriLiveLinkSourceFactory : public ULiveLinkSourceFactory
{
public:
	GENERATED_BODY()

	virtual FText GetSourceDisplayName() const override;
	virtual FText GetSourceTooltip() const override;

	virtual EMenuType GetMenuType() const override { return EMenuType::SubPanel; }
	virtual TSharedPtr<SWidget> BuildCreationPanel(FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const override;
	virtual TSharedPtr<ILiveLinkSource> CreateSource(const FString& ConnectionString) const override;

private:
	void CreateSourceFromSettings(FArriLiveLinkConnectionSettings ConnectionSettings, FOnLiveLinkSourceCreated OnSourceCreated) const;
};
