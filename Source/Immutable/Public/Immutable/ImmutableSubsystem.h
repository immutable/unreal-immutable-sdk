﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ImmutableSubsystem.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FImmutableSubsystemReadyDelegate, TWeakObjectPtr<class UImtblJSConnector>);

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImmutableSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

    friend class UImmutablePassport;
    friend class UImtblPassportInitializationAsyncAction;
    friend class UImtblPassportHasStoredCredentialsAsyncAction;
    friend class UImtblPassportConnectSilentAsyncAction;
    friend class UImtblPassportConnectAsyncAction;
    friend class UImtblPassportConnectEvmAsyncAction;
    friend class UImtblPassportZkEvmRequestAccountsAsyncAction;
    friend class UImtblPassportZkEvmGetBalanceAsyncAction;
    friend class UImtblPassportZkEvmSendTransactionAsyncAction;
    friend class UImtblPassportLogoutAsyncAction;
    friend class UImtblPassportGetAddressAsyncAction;
    friend class UImtblPassportGetEmailAsyncAction;

public:
    UImmutableSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

    TWeakObjectPtr<class UImmutablePassport> GetPassport() const { return MakeWeakObjectPtr(Passport); }

    bool IsReady() const { return bIsReady; }

    // FOnGameViewportTick& OnGameViewportTick() { return GameViewportTickEvent; }

protected:
    // Execute a delegate when the subsystem is ready (i.e.: when the browser is running and the Immutable SDK game bridge has loaded).
    template<class UserClass>
    void WhenReady(UserClass* Object, typename FImmutableSubsystemReadyDelegate::FDelegate::TUObjectMethodDelegate<UserClass>::FMethodPtr Func);

private:
	UPROPERTY()
	class UImtblBrowserUserWidget* BrowserWidget = nullptr;
    
	UPROPERTY()
	class UImtblBlui* ImtblBlui = nullptr;
    
    UPROPERTY()
    class UImmutablePassport* Passport = nullptr;

    bool bHasSetupGameBridge = false;
    bool bIsReady = false;
    FImmutableSubsystemReadyDelegate OnReady;

    FDelegateHandle WorldTickHandle;
    FDelegateHandle ViewportCreatedHandle;
#if PLATFORM_ANDROID | PLATFORM_IOS
    FDelegateHandle EngineInitCompleteHandle;
#endif
	
    void SetupGameBridge();
	void OnBridgeReady();
    void ManageBridgeDelegateQueue();
    void OnViewportCreated();
    void WorldTickStart(UWorld* World, ELevelTick TickType, float DeltaSeconds);
};
