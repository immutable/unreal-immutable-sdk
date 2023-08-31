// Fill out your copyright notice in the Description page of Project Settings.


#include "ImtblBlui.h"
#include "ImtblJSConnector.h"
#include "Immutable/Misc/ImtblLogging.h"

#if USING_BLUI_CEF
#include "BluEye.h"
#endif

UImtblBlui::UImtblBlui()
{
#if USING_BLUI_CEF
	if (!BluEyePtr)
	{
		IMTBL_LOG("Creating BluEye")
		BluEyePtr = NewObject<UBluEye>(this, "BluEye");
	}
#endif

	JSConnector = NewObject<UImtblJSConnector>(this, "JSConnector");
	JSConnector->ExecuteJs = UImtblJSConnector::FOnExecuteJsDelegate::CreateUObject(this, &UImtblBlui::ExecuteJS);
}

#if USING_BLUI_CEF
UBluEye* UImtblBlui::GetBluEye() const
{
	return Cast<UBluEye>(BluEyePtr);
}
#endif

void UImtblBlui::OnLogEvent(const FString& LogText)
{
	IMTBL_LOG_FUNC("LogEvent: %s",*LogText);
}

void UImtblBlui::OnScriptEvent(const FString& EventName, const FString& EventMessage)
{
	IMTBL_LOG_FUNC("EventName: %s, EventMessage: %s", *EventName, *EventMessage);
	JSConnector->SendToGame(EventMessage);
}

TWeakObjectPtr<UImtblJSConnector> UImtblBlui::GetJSConnector() const
{
	return JSConnector;
}

void UImtblBlui::ExecuteJS(const FString& ScriptText) const
{
#if USING_BLUI_CEF
	if (GetBluEye())
	{
		GetBluEye()->ExecuteJS(ScriptText);
	}
#endif
}

void UImtblBlui::Init()
{
	IMTBL_LOG_FUNCSIG

#if USING_BLUI_CEF
	// Todo: Add comment here why GetBluEye
	UBluEye* BluEye = GetBluEye();

	BluEye->LogEventEmitter.AddUniqueDynamic(this, &UImtblBlui::OnLogEvent);
	BluEye->ScriptEventEmitter.AddUniqueDynamic(this, &UImtblBlui::OnScriptEvent);

	BluEye->bEnabled = true;
	IMTBL_LOG("Events subscribed")

	BluEye->Init();
	IMTBL_LOG("BluEye Initialised")

	FSoftObjectPath AssetRef(TEXT("/Script/Immutable.ImtblSDKResource'/Immutable/PackagedResources/index.index'"));
    if (UObject* LoadedAsset = AssetRef.TryLoad())
    {
        if (auto Resource = Cast<UImtblSDKResource>(LoadedAsset))
        {
			// const FString Html = "<html><head></head><body><div>Stuff IN A WEBSITE</div><script>blu_event('blah', 'BALKJSDLFKJDSLKFJDSLDSFL');</script></body></html>";
			const FString DataUrl = "data:text/html;charset=utf-8," + Resource->Data;
			BluEye->LoadURL(*DataUrl);
			IMTBL_LOG("DataUrl loaded")
		}
	}

	// const FString LocalHtmlFile = "blui://" + FString("Content/html/index.html");
	// BluEye->LoadURL(LocalHtmlFile);
	// UE_LOG(LogTemp, Log, TEXT("Game Bridge Loaded"));

	// Do this after the the page is given to the browser and being loaded...
	JSConnector->Init(!BluEye->IsBrowserLoading());
#endif
}
