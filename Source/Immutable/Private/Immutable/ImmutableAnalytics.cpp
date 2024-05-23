#include "ImmutableAnalytics.h"

#include "JsonDomBuilder.h"

#define EVENT_DATA_SUCCESS "succeeded"


void UImmutableAnalytics::Track(EEventName EventName)
{
	check(JSConnector.IsValid());
	
	FString FullData;
	
	FJsonObjectConverter::UStructToJsonObjectString<FEventData>({ "unrealSdk", GetEventName(EventName), "" }, FullData);

	JSConnector->CallJS(ImmutablePassportAction::TRACK, FullData, FImtblJSResponseDelegate::CreateUObject(this, &UImmutableAnalytics::OnTrackResponse));
}

void UImmutableAnalytics::Track(EEventName EventName, bool Success)
{
	TMap<FString, FString> EventData = {{EVENT_DATA_SUCCESS, Success ? TEXT("true") : TEXT("false") } };
	Track(EventName, EventData);
}

void UImmutableAnalytics::Track(EEventName EventName, TMap<FString, FString>& EventData)
{
	check(JSConnector.IsValid());

	FJsonDomBuilder::FObject Object;

	for (auto data : EventData)
	{
		Object.Set(data.Key, data.Value);	
	}

	FString FullData;
	
	FJsonObjectConverter::UStructToJsonObjectString<FEventData>({ "unrealSdk", GetEventName(EventName), Object.ToString() }, FullData);

	JSConnector->CallJS(ImmutablePassportAction::TRACK, FullData, FImtblJSResponseDelegate::CreateUObject(this, &UImmutableAnalytics::OnTrackResponse));
}

void UImmutableAnalytics::Setup(TWeakObjectPtr<UImtblJSConnector> Connector)
{	
	IMTBL_LOG_FUNCSIG

	if (!Connector.IsValid())
	{
		IMTBL_ERR("Invalid JSConnector passed to UImmutableAnalytics::Setup.")
		return;
	}

	JSConnector = Connector.Get();
}

void UImmutableAnalytics::OnTrackResponse(FImtblJSResponse Response)
{
	// Currently, we ignore tracking errors. Edit if it is required
}

FString UImmutableAnalytics::GetEventName(EEventName EventName)
{
	switch(EventName)
	{
	#define CONVERT(EventName, EventNameString) case EEventName::EventName: return EventNameString;
	EVENT_NAME_LIST
	#undef CONVERT
	default:
		return "";
	}
}
