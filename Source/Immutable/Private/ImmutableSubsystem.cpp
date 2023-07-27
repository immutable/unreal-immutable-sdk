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

    StartHandle = FWorldDelegates::OnStartGameInstance.AddLambda([this](UGameInstance* GameInstance)
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
            IMTBL_ERR("JSConnector not available, can't set up ready event chain")
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
    });

    TearDownHandle = FWorldDelegates::OnWorldBeginTearDown.AddLambda([this](UWorld* World)
    {
        IMTBL_LOG_FUNC("OnWorldBeginTearDown")
    });

    CleanupHandle = FWorldDelegates::OnWorldCleanup.AddLambda([this](UWorld* World, bool bSessionEnded, bool bCleanupResources)
    {
        IMTBL_LOG_FUNC("OnWorldCleanup")
    });
}


void UImmutableSubsystem::Deinitialize()
{
    IMTBL_LOG_FUNCSIG
    BrowserWidget = nullptr;
    Passport = nullptr;

    FWorldDelegates::OnStartGameInstance.Remove(StartHandle);
    FWorldDelegates::OnWorldBeginTearDown.Remove(TearDownHandle);
    FWorldDelegates::OnWorldCleanup.Remove(CleanupHandle);

    Super::Deinitialize();
}


template <class UserClass>
void UImmutableSubsystem::WhenReady(UserClass* Object, typename FImmutableSubsystemReadyDelegate::FDelegate::TUObjectMethodDelegate<UserClass>::FMethodPtr Func, float ReadyTimeout)
{
    if (IsReady())
    {
        // Defer to next tick so we're not blocking the caller
        if (GetWorld())
        {
            OnReady.AddUObject(Object, Func);
            GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UImmutableSubsystem::OnBridgeReady);
        }
        else
        {
            // Can't defer to next tick, so just call the delegate now
            FImmutableSubsystemReadyDelegate::FDelegate::CreateUObject(Object, Func).ExecuteIfBound(BrowserWidget->GetJSConnector());
        }
    }
    else
    {
        FDelegateHandle Handle = OnReady.AddUObject(Object, Func);

        // Implement timeout (if there is a world to get a timer from)
        if (ReadyTimeout > 0.0f && GetWorld())
        {
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, Handle]()
            {
                if (!IsReady())
                {
                    IMTBL_WARN("WhenReady timed out");
                    OnReady.Remove(Handle);
                    // TODO: add a timeout callback
                }
            }, ReadyTimeout, false);
        }
    }
}


void UImmutableSubsystem::OnBridgeReady()
{
    // When the bridge is ready our subsystem is ready to be used by game code.
    // Set the bIsReady flag and broadcast the OnReady event for any waiting delegates.
    bIsReady = true;
    OnReady.Broadcast(BrowserWidget->GetJSConnector());
    OnReady.Clear();
}
