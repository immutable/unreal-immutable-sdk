// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportZkEvmRequestAccountsAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImtblPassportZkEvmRequestAccountsAsyncAction : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPassportZkEvmRequestAccountsOutputPin, FString, ErrorMessage, const TArray<FString> &, Accounts);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblPassportZkEvmRequestAccountsAsyncAction* RequestAccounts(UObject* WorldContextObject);

	virtual void Activate() override;

private:
	void DoRequestAccounts(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
	void OnRequestAccountsResponse(FImmutablePassportResult Result);

	UPROPERTY(BlueprintAssignable)
	FPassportZkEvmRequestAccountsOutputPin Failed;
	UPROPERTY(BlueprintAssignable)
	FPassportZkEvmRequestAccountsOutputPin GotAccounts;
};
