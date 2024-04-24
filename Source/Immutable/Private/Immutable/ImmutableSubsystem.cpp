// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/ImmutableSubsystem.h"

#include "Blueprint/UserWidget.h"
#include "Immutable/ImmutablePassport.h"
#include "Immutable/Misc/ImtblLogging.h"
#include "ImtblBlui.h"
#include "ImtblBrowserUserWidget.h"
#include "Immutable/ImtblJSConnector.h"

UImmutableSubsystem::UImmutableSubsystem() { IMTBL_LOG_FUNCSIG }

void UImmutableSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	IMTBL_LOG_FUNCSIG
	Super::Initialize(Collection);

#if PLATFORM_ANDROID
  // Enable DOM storage so we can use localStorage in the Android webview
  GConfig->SetBool(
      TEXT("/Script/AndroidRuntimeSettings.AndroidRuntimeSettings"),
      TEXT("bEnableDomStorage"), true, GEngineIni);
#endif

#if PLATFORM_ANDROID | PLATFORM_IOS
  EngineInitCompleteHandle =
      FCoreDelegates::OnFEngineLoopInitComplete.AddUObject(
          this, &UImmutableSubsystem::OnViewportCreated);
#else
	ViewportCreatedHandle = UGameViewportClient::OnViewportCreated().AddUObject(this, &UImmutableSubsystem::OnViewportCreated);
#endif
	WorldTickHandle = FWorldDelegates::OnWorldTickStart.AddUObject(this, &UImmutableSubsystem::WorldTickStart);
}

void UImmutableSubsystem::Deinitialize()
{
	IMTBL_LOG_FUNCSIG
	
	BrowserWidget = nullptr;
	
#if USING_BLUI_CEF
	ImtblBlui->ConditionalBeginDestroy();
	ImtblBlui = nullptr;
#endif
	
	Passport = nullptr;
	
#if PLATFORM_ANDROID | PLATFORM_IOS
  UGameViewportClient::OnViewportCreated().Remove(EngineInitCompleteHandle);
#else
	UGameViewportClient::OnViewportCreated().Remove(ViewportCreatedHandle);
#endif
	FWorldDelegates::OnWorldTickStart.Remove(WorldTickHandle);

	Super::Deinitialize();
}

template <class UserClass>
#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
void UImmutableSubsystem::WhenReady(UserClass* Object, typename FImmutableSubsystemReadyDelegate::FDelegate::TMethodPtr<UserClass> Func)
#else
void UImmutableSubsystem::WhenReady(UserClass* Object, typename FImmutableSubsystemReadyDelegate::FDelegate::TUObjectMethodDelegate<UserClass>::FMethodPtr Func)
#endif
{
	OnReady.AddUObject(Object, Func);
}

void UImmutableSubsystem::OnBridgeReady()
{
	// When the bridge is ready our subsystem is ready to be used by game code.
	// Set the bIsReady flag and broadcast the OnReady event for any waiting
	// delegates.
	bIsReady = true;
	ManageBridgeDelegateQueue();
}

void UImmutableSubsystem::ManageBridgeDelegateQueue()
{
	if (bIsReady)
	{
#if USING_BLUI_CEF
    OnReady.Broadcast(ImtblBlui->GetJSConnector());
#else
		OnReady.Broadcast(BrowserWidget->GetJSConnector());
#endif
		OnReady.Clear();
	}
}

void UImmutableSubsystem::SetupGameBridge()
{
	if (bHasSetupGameBridge)
	{
		return;
	}
	bHasSetupGameBridge = true;

#if USING_BLUI_CEF
  // Create the Blui
  if (!ImtblBlui) {
    ImtblBlui = NewObject<UImtblBlui>();
    ImtblBlui->Init();
  }

  if (!ImtblBlui) {
    IMTBL_ERR("Failed to create UImtblBlui")
    return;
  }
  if (!ImtblBlui->GetJSConnector().IsValid()) {
    IMTBL_ERR(
        "JSConnector not available, can't set up subsystem-ready event chain")
    return;
  }
  // Set up ready event chain
  if (!IsReady()) {
    ImtblBlui->GetJSConnector()->AddCallbackWhenBridgeReady(
        UImtblJSConnector::FOnBridgeReadyDelegate::FDelegate::CreateUObject(
            this, &UImmutableSubsystem::OnBridgeReady));
  }

  // Prepare Passport
  if (!Passport) {
    Passport = NewObject<UImmutablePassport>(this);
    if (Passport)
      Passport->Setup(ImtblBlui->GetJSConnector());
  }

#else
	// Create the browser widget
	if (!BrowserWidget)
	{
		BrowserWidget = CreateWidget<UImtblBrowserUserWidget>(GetWorld());
	}
	if (!BrowserWidget)
	{
		IMTBL_ERR("Failed to create up BrowserWidget")
		return;
	}
	// Launch browser
	if (!BrowserWidget->IsInViewport())
	{
		IMTBL_LOG("Adding BrowserWidget to viewport")
		BrowserWidget->AddToViewport();
	}
	if (!BrowserWidget->GetJSConnector().IsValid())
	{
		IMTBL_ERR("JSConnector not available, can't set up subsystem-ready event chain")
		return;
	}
	// Set up ready event chain
	if (!IsReady())
	{
		BrowserWidget->GetJSConnector()->AddCallbackWhenBridgeReady(UImtblJSConnector::FOnBridgeReadyDelegate::FDelegate::CreateUObject(this, &UImmutableSubsystem::OnBridgeReady));
	}

	// Prepare Passport
	if (!Passport)
	{
		Passport = NewObject<UImmutablePassport>(this);
		if (Passport)
		{
			Passport->Setup(BrowserWidget->GetJSConnector());
		}
	}
#endif
}

void UImmutableSubsystem::OnViewportCreated()
{
	IMTBL_LOG_FUNCSIG
	SetupGameBridge();
}

void UImmutableSubsystem::WorldTickStart(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
	ManageBridgeDelegateQueue();
}
