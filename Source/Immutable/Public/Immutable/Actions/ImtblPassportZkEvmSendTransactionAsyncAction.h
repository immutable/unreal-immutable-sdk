// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportZkEvmSendTransactionAsyncAction.generated.h"

/**
 * Async action blueprint node for zkEVM Send Transaction
 */
UCLASS()
class IMMUTABLE_API UImtblPassportZkEvmSendTransactionAsyncAction : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPassportZkEvmSendTransactionOutputPin, FString, ErrorMessage, FString, Transaction);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblPassportZkEvmSendTransactionAsyncAction* ZkEvmSendTransaction(UObject* WorldContextObject, const FImtblTransactionRequest& Request);

	virtual void Activate() override;

private:
	FImtblTransactionRequest TransactionRequest;

	UPROPERTY(BlueprintAssignable)
	FPassportZkEvmSendTransactionOutputPin TransactionSent;
	UPROPERTY(BlueprintAssignable)
	FPassportZkEvmSendTransactionOutputPin Failed;

	void DoZkEvmSendTransaction(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
	void OnZkEvmSendTransactionResponse(FImmutablePassportResult Result);
};


/**
 * Async action blueprint node for zkEVM Send Transaction with confirmation.
 * Allows bypassing the need to call 'get transaction receipt' in order to confirm the transaction.
 */
UCLASS()
class IMMUTABLE_API UImtblPassportZkEvmSendTransactionWithConfirmationAA : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPassportZkEvmSendTransactionWithConfirmationOutputPin, FString, ErrorMessage, const struct FZkEvmTransactionReceipt&, Receipt);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblPassportZkEvmSendTransactionWithConfirmationAA* ZkEvmSendTransactionWithConfirmation(UObject* WorldContextObject, const FImtblTransactionRequest& Request);

	virtual void Activate() override;

private:
	FImtblTransactionRequest TransactionRequest;

	UPROPERTY(BlueprintAssignable)
	FPassportZkEvmSendTransactionWithConfirmationOutputPin Success;
	UPROPERTY(BlueprintAssignable)
	FPassportZkEvmSendTransactionWithConfirmationOutputPin Failed;

	void DoZkEvmSendTransactionWithConfirmation(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
	void OnZkEvmSendTransactionWithConfirmationResponse(FImmutablePassportResult Result);
};
