// Fill out your copyright notice in the Description page of Project Settings.

#include "ImtblBlui.h"
#include "Immutable/Misc/ImtblLogging.h"
#include "Immutable/ImtblJSConnector.h"
#include "Immutable/ImmutableUtilities.h"
#include <utility>

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
UBluEye *UImtblBlui::GetBluEye() const { return Cast<UBluEye>(BluEyePtr); }
#endif

void UImtblBlui::OnLogEvent(const FString& LogText)
{
	IMTBL_LOG_FUNC("LogEvent: %s", *LogText);
}

void UImtblBlui::WorldTickStart(UWorld* World, ELevelTick LevelTick, float X)
{
#if USING_BLUI_CEF
	if (!GetBluEye()->IsBrowserLoading() && !bLoadedIndexJS)
	{
		FString JavaScript;
		
		IMTBL_LOG("BLUI CEF Browser loaded");
		bLoadedIndexJS = true;
		if (FImmutableUtilities::LoadGameBridge(JavaScript))
		{
			GetBluEye()->ExecuteJS(JavaScript);
		}
	}
#endif
}

void UImtblBlui::BeginDestroy()
{
#if USING_BLUI_CEF
	if (GetBluEye())
	{
		GetBluEye()->CloseBrowser();
	}
	BluEyePtr = nullptr;
#endif
	Super::BeginDestroy();
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
#if USING_BLUI_CEF
	UBluEye *BluEye = GetBluEye();

	BluEye->LogEventEmitter.AddUniqueDynamic(this, &UImtblBlui::OnLogEvent);
	BluEye->ScriptEventEmitter.AddUniqueDynamic(this, &UImtblBlui::OnScriptEvent);

	BluEye->bEnabled = true;
	IMTBL_LOG("Events subscribed")

	if (!BluEye->Init())
	{
		IMTBL_ERR("BluEye is not initialised")
		return;
	}
	IMTBL_LOG("BluEye Initialised")

	// We're attempting to replicate the process that Unreal's WebBrowser
	// widget uses to load a page from a string. Unfortunately this doesn't
	// work correctly, but it still solves our issue. LocalStorage can't be
	// accessed from about:blank or data URIs, so we still need to load a
	// page.  Despite this failing to load our custom html, this approach
	// still allows us to access LocalStorage and use the game bridge.  If
	// there was more time in the future it would probably be worth
	// investigating the issues here.

	// PostData
	CefRefPtr<CefPostData> PostData = CefPostData::Create();
	CefRefPtr<CefPostDataElement> Element = CefPostDataElement::Create();
	FTCHARToUTF8 UTF8String(TEXT(
	  "<!doctype html><html lang='en'><head><meta "
	  "charset='utf-8'><title>GameSDK "
	  "Bridge</title></head><body><h1>Bridge Running</h1></body></html>"));
	Element->SetToBytes(UTF8String.Length(), UTF8String.Get());
	PostData->AddElement(Element);

	CefRequest::HeaderMap HeaderMap;
	HeaderMap.insert(std::pair<CefString, CefString>(
	  TCHAR_TO_WCHAR(TEXT("Content-Type")), "html"));

	const FString CustomContentMethod(TEXT("X-GET-CUSTOM-CONTENT"));

	const auto Request = CefRequest::Create();
	Request->Set("file:///Immutable/index.html", *CustomContentMethod,
	           PostData, HeaderMap);

	GetBluEye()->Browser->GetMainFrame()->LoadRequest(Request);
	IMTBL_VERBOSE("LoadRequest'ed for Index.html")

	WorldTickHandle = FWorldDelegates::OnWorldTickStart.AddUObject(
	  this, &UImtblBlui::WorldTickStart);
	IMTBL_LOG("Waiting for Browser to load...");

	// Do this after the the page is given to the browser and being loaded...
	JSConnector->Init(!BluEye->IsBrowserLoading());
#endif
}

#if USING_BLUI_CEF
void UImtblBlui::StopBluiEventLoop()
{
	if (UBluEye* BluEye = GetBluEye())
	{
		BluEye->SetShouldTickEventLoop(false);
	}
}
#endif
