// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"

#include "ImtblPassportZkEvmGetTransactionReceiptAA.generated.h"


/**
 *
 */
UCLASS()
class IMMUTABLE_API UImtblPassportZkEvmGetTransactionReceiptAA : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FZkEvmGetTransactionReceiptOutputPin, FString, ErrorMessage, const struct FZkEvmTransactionReceipt&, Receipt);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblPassportZkEvmGetTransactionReceiptAA* ZkEvmGetTransactionReceipt(UObject* WorldContextObject, const FString& Hash);

	virtual void Activate() override;

private:
	FString Hash;

	UPROPERTY(BlueprintAssignable)
	FZkEvmGetTransactionReceiptOutputPin Success;
	UPROPERTY(BlueprintAssignable)
	FZkEvmGetTransactionReceiptOutputPin Failed;

	void DoZkEvmGetTransactionReceipt(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
	void OnZkEvmGetTransactionReceiptResponse(FImmutablePassportResult Result);
};
