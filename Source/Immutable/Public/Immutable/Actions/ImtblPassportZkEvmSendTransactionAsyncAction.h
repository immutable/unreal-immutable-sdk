// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportZkEvmSendTransactionAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImtblPassportZkEvmSendTransactionAsyncAction
    : public UImtblBlueprintAsyncAction {
  GENERATED_BODY()

  DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
      FPassportZkEvmSendTransactionOutputPin, FString, ErrorMessage, FString,
      Transaction);

public:
  UFUNCTION(BlueprintCallable,
            meta = (WorldContext = "WorldContextObject",
                    BlueprintInternalUseOnly = "true"),
            Category = "Immutable")
  static UImtblPassportZkEvmSendTransactionAsyncAction *
  ZkEvmSendTransaction(UObject *WorldContextObject,
                       const FImtblTransactionRequest &Request);

  void Activate() override;

private:
  FImtblTransactionRequest TransactionRequest;

  UPROPERTY(BlueprintAssignable)
  FPassportZkEvmSendTransactionOutputPin TransactionSent;
  UPROPERTY(BlueprintAssignable)
  FPassportZkEvmSendTransactionOutputPin Failed;

  void DoZkEvmSendTransaction(
      TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
  void OnZkEvmSendTransactionResponse(FImmutablePassportResult Result);
};