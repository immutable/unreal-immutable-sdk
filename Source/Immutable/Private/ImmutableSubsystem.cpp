// Fill out your copyright notice in the Description page of Project Settings.


#include "ImmutableSubsystem.h"

#include "ImmutablePassport.h"
#include "ImtblBrowserUserWidget.h"
#include "ImtblJSConnector.h"
#include "ImtblLogging.h"
#include "Blueprint/UserWidget.h"


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
}


void UImmutableSubsystem::WorldTickStart(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
    ManageBridgeDelegateQueue();
}
