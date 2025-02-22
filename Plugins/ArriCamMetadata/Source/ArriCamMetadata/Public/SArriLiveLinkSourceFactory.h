# pragma once

#include "ArriLiveLinkConnectionSettings.h"
#include "Widgets/SCompoundwidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

#if WITH_EDITOR
#include "IStructureDetailsView.h"
#endif //WITH_EDITOR

#include "Input/Reply.h"

struct FArriLiveLinkConnectionSettings;

DECLARE_DELEGATE_OneParam(FOnArriLiveLinkConnectionSettingsAccepted, FArriLiveLinkConnectionSettings);

class SArriLiveLinkSourceFactory : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SArriLiveLinkSourceFactory)
	{}
		SLATE_EVENT(FOnArriLiveLinkConnectionSettingsAccepted, OnConnectionSettingsAccepted)
	SLATE_END_ARGS()

		void Construct(const FArguments& Args);


private:
	FArriLiveLinkConnectionSettings ConnectionSettings;

#if WITH_EDITOR
	TSharedPtr<FStructOnScope> StructOnScope;
	TSharedPtr<IStructureDetailsView> StructureDetailsView;
#endif //WITH_EDITOR

	FReply OnSettingsAccepted();
	FOnArriLiveLinkConnectionSettingsAccepted OnConnectionSettingsAccepted;
};