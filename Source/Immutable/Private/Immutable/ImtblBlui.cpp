// Fill out your copyright notice in the Description page of Project Settings.


#include "ImtblBlui.h"

#include "BluEye.h"
#include "ImtblJSConnector.h"
#include "Immutable/Misc/ImtblLogging.h"


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
	UE_LOG(LogTemp, Log, TEXT("UImtblBlui::Init()"));

#if USING_BLUI_CEF
	// Todo: Add comment here why GetBluEye
	UBluEye* BluEye = GetBluEye();

	BluEye->LogEventEmitter.AddUniqueDynamic(this, &UImtblBlui::OnLogEvent);
	BluEye->ScriptEventEmitter.AddUniqueDynamic(this, &UImtblBlui::OnScriptEvent);

	BluEye->bEnabled = true;
	UE_LOG(LogTemp, Log, TEXT("Events subscribed"))

	BluEye->Init();
	UE_LOG(LogTemp, Log, TEXT("BluEye Initialised"))

	// Todo: convert the index.html file to be loaded as an .uasset.

	//BluEye->LoadURL(FString::Printf(TEXT("%s%s"), TEXT("file://"), *FPaths::Combine(FPaths::ProjectContentDir(), TEXT("html"), TEXT("index.html"))));

	const FString LocalHtmlFile = "blui://" + FString("Content/html/index.html");
	BluEye->LoadURL(LocalHtmlFile);
	UE_LOG(LogTemp, Log, TEXT("Game Bridge Loaded"));

	// Do this after the the page is given to the browser and being loaded...
	JSConnector->Init(!BluEye->IsBrowserLoading());
#endif
}
