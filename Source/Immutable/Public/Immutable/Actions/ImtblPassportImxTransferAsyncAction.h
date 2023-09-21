// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportImxTransferAsyncAction.generated.h"

/**
 * 
 */
UCLASS()
class IMMUTABLE_API UImmutablePassportImxTransferAsyncAction : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPassportImxTransferIdOutputPin, FString, ErrorMessageString, FString, TransferId);
    
public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImmutablePassportImxTransferAsyncAction* Transfer(UObject* WorldContextObject,
		const FString& Receiver, const FString& Type, const FString& Amount, const FString& TokenId,
		const FString& TokenAddress);

	virtual void Activate() override;

private:
	void DoTransfer(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
	void OnTransferResponse(FImmutablePassportResult Result);

	FString Receiver;
	FString Type;
	FString Amount;
	FString TokenId;
	FString TokenAddress;
	
	UPROPERTY(BlueprintAssignable)
	FPassportImxTransferIdOutputPin Success;
	UPROPERTY(BlueprintAssignable)
	FPassportImxTransferIdOutputPin Failed;
};
