// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/EngineBaseTypes.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "ImmutablePassport.h"
// clang-format off
#include "ImmutableSubsystem.generated.h"
// clang-format on

DECLARE_MULTICAST_DELEGATE_OneParam(FImmutableSubsystemReadyDelegate, TWeakObjectPtr<class UImtblJSConnector>);

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImmutableSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	TWeakObjectPtr<UImmutablePassport> GetPassport() const
	{
		return MakeWeakObjectPtr(Passport);
	}

	bool IsReady() const { return bIsReady; }
	FImmutableSubsystemReadyDelegate* OnReady();

	// Execute a delegate when the subsystem is ready (i.e.: when the browser is
	// running and the Immutable SDK game bridge has loaded).
	template <class UserClass>
#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
	void WhenReady(UserClass* Object, typename FImmutableSubsystemReadyDelegate::FDelegate::TMethodPtr<UserClass> Func)
#else
  void WhenReady(UserClass* Object, typename FImmutableSubsystemReadyDelegate::FDelegate::TUObjectMethodDelegate<UserClass>::FMethodPtr Func)
#endif
	{
		MulticastDelegate_OnReady.AddUObject(Object, Func);
	}

private:
	void SetupGameBridge();
	void OnBridgeReady();
	void ManageBridgeDelegateQueue();
	void OnViewportCreated();
	void WorldTickStart(UWorld* World, ELevelTick TickType, float DeltaSeconds);

private:
	UPROPERTY()
	class UImtblBrowserUserWidget* BrowserWidget = nullptr;

	UPROPERTY()
	class UImtblBlui* ImtblBlui = nullptr;

	UPROPERTY()
	UImmutablePassport* Passport = nullptr;

	bool bHasSetupGameBridge = false;
	bool bIsReady = false;
	FImmutableSubsystemReadyDelegate MulticastDelegate_OnReady;

	FDelegateHandle WorldTickHandle;
	FDelegateHandle ViewportCreatedHandle;
#if PLATFORM_ANDROID | PLATFORM_IOS
	FDelegateHandle EngineInitCompleteHandle;
#endif
};