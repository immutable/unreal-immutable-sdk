// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportZkEvmGetBalanceAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImtblPassportZkEvmGetBalanceAsyncAction
    : public UImtblBlueprintAsyncAction {
  GENERATED_BODY()

  DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
      FPassportZkEvmGetBalanceOutputPin, FString, ErrorMessage, FString,
      Balance);

public:
  UFUNCTION(BlueprintCallable,
            meta = (WorldContext = "WorldContextObject",
                    BlueprintInternalUseOnly = "true"),
            Category = "Immutable")
  static UImtblPassportZkEvmGetBalanceAsyncAction *
  ZkEvmGetBalance(UObject *WorldContextObject, const FString &Address,
                  const FString &BlockNumberOrTag);

  void Activate() override;

private:
  FString Address;
  FString BlockNumberOrTag;

  UPROPERTY(BlueprintAssignable)
  FPassportZkEvmGetBalanceOutputPin GotBalance;
  UPROPERTY(BlueprintAssignable)
  FPassportZkEvmGetBalanceOutputPin Failed;

  void
  DoZkEvmGetBalance(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
  void OnZkEvmGetBalanceResponse(FImmutablePassportResult Result);
};
