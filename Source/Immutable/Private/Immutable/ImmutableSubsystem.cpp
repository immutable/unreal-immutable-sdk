// Fill out your copyright notice in the Description page of Project Settings.


#include "Immutable/ImmutableSubsystem.h"

#include "Immutable/ImmutablePassport.h"
#include "ImtblBrowserUserWidget.h"
#include "ImtblJSConnector.h"
#include "Immutable/Misc/ImtblLogging.h"
#include "Blueprint/UserWidget.h"

#if USING_BLUI_CEF
#include "BluEye.h"
#endif


void UImmutableSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    IMTBL_LOG_FUNCSIG
    Super::Initialize(Collection);

    StartHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, &UImmutableSubsystem::StartGameInstance);

    WorldTickHandle = FWorldDelegates::OnWorldTickStart.AddUObject(this, &UImmutableSubsystem::WorldTickStart);
}


void UImmutableSubsystem::Deinitialize()
{
    IMTBL_LOG_FUNCSIG
    BrowserWidget = nullptr;
    Passport = nullptr;

    FWorldDelegates::OnStartGameInstance.Remove(StartHandle);
    FWorldDelegates::OnWorldTickStart.Remove(WorldTickHandle);
    
    Super::Deinitialize();
}


template <class UserClass>
void UImmutableSubsystem::WhenReady(UserClass* Object, typename FImmutableSubsystemReadyDelegate::FDelegate::TUObjectMethodDelegate<UserClass>::FMethodPtr Func)
{
    OnReady.AddUObject(Object, Func);
}


#if USING_BLUI_CEF
UBluEye* UImmutableSubsystem::GetBluEye()
{
	if (!TheBluEye)
	{
		IMTBL_LOG("Creating BluEye")
		TheBluEye = NewObject<UBluEye>(this);
	}
	return Cast<UBluEye>(TheBluEye);
}
#endif


void UImmutableSubsystem::OnBridgeReady()
{
    // When the bridge is ready our subsystem is ready to be used by game code.
    // Set the bIsReady flag and broadcast the OnReady event for any waiting delegates.
    bIsReady = true;
    ManageBridgeDelegateQueue();
}


void UImmutableSubsystem::ManageBridgeDelegateQueue()
{
    if (bIsReady)
    {
        OnReady.Broadcast(BrowserWidget->GetJSConnector());
        OnReady.Clear();
    }
}


void UImmutableSubsystem::StartGameInstance(UGameInstance* GameInstance)
{
    IMTBL_LOG_FUNC("OnStartGameInstance")

#if USING_BLUI_CEF
    UE_LOG(LogTemp, Log, TEXT("BeginPlay()"))

    UBluEye* BluEye = GetBluEye();
	UE_LOG(LogTemp, Log, TEXT("BluEye created"))
	
	// BluEye->LogEventEmitter.AddUniqueDynamic(this, &AImtblUnrealSampleGameModeBase::OnLogEvent);
	// BluEye->ScriptEventEmitter.AddUniqueDynamic(this, &AImtblUnrealSampleGameModeBase::OnScriptEvent);
	BluEye->bEnabled = true;
	UE_LOG(LogTemp, Log, TEXT("Events subscribed"))
	
	BluEye->Init();
	UE_LOG(LogTemp, Log, TEXT("BluEye Init()ed"))
	
	const FString Html = "<html><head></head><body><div>Stuff IN A WEBSITE</div><script>blu_event('blah', 'BALKJSDLFKJDSLKFJDSLDSFL');</script></body></html>";
	const FString DataUrl = "data:text/html;charset=utf-8," + Html;
	BluEye->LoadURL(*DataUrl);
	UE_LOG(LogTemp, Log, TEXT("DataUrl loaded"))
	
	// GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AImtblUnrealSampleGameModeBase::WhenBrowserReady);

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
        BrowserWidget->GetJSConnector()->WhenBridgeReady(UImtblJSConnector::FOnBridgeReadyDelegate::FDelegate::CreateUObject(this, &UImmutableSubsystem::OnBridgeReady));
    }
	
    // Prepare Passport
    if (!Passport)
    {
        Passport = NewObject<UImmutablePassport>(this);
        if (Passport)
            Passport->Setup(BrowserWidget->GetJSConnector());
    }
#endif
}


void UImmutableSubsystem::WorldTickStart(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
    ManageBridgeDelegateQueue();
}

