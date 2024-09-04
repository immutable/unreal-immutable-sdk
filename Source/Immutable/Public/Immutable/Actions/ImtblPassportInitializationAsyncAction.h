// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"

#include "ImtblPassportInitializationAsyncAction.generated.h"

/**
 * Async action to instantiate  
 */
UCLASS()
class IMMUTABLE_API UImtblPassportInitializationAsyncAction : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPassportInitializationOutputPin, FString, Message);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblPassportInitializationAsyncAction* InitializePassport(UObject* WorldContextObject, const FString& ClientID, const FString& RedirectUri, const FString& LogoutUri, const FString& Environment, bool IsSilentLogout);

	virtual void Activate() override;

private:
	FString ClientId;
	FString RedirectUri;
	FString LogoutUri;
	FString Environment;
	bool IsSilentLogout;

	UPROPERTY(BlueprintAssignable)
	FPassportInitializationOutputPin Initialized;
	UPROPERTY(BlueprintAssignable)
	FPassportInitializationOutputPin Failed;

	void DoInit(TWeakObjectPtr<class UImtblJSConnector> JSConnector);
	void OnInitialized(FImmutablePassportResult Result);
};
