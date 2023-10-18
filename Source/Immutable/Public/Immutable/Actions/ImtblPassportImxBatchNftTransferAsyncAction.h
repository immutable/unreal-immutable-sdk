// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportImxBatchNftTransferAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImmutablePassportImxBatchNftTransferAsyncAction
    : public UImtblBlueprintAsyncAction {
  GENERATED_BODY()

  DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
      FPassportImxBatchNftTransferIdOutputPin, FString, ErrorMessageString,
      const TArray<FString> &, TransferIds);

public:
  UFUNCTION(BlueprintCallable,
            meta = (WorldContext = "WorldContextObject",
                    BlueprintInternalUseOnly = "true"),
            Category = "Immutable")
  static UImmutablePassportImxBatchNftTransferAsyncAction *
  ImxBatchNftTransfer(UObject *WorldContextObject,
                      const TArray<FNftTransferDetails> &NftTransferDetails);

  void Activate() override;

private:
  void DoTransfer(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
  void OnTransferResponse(FImmutablePassportResult Result);

  TArray<FNftTransferDetails> NftTransferDetails;

  UPROPERTY(BlueprintAssignable)
  FPassportImxBatchNftTransferIdOutputPin Success;
  UPROPERTY(BlueprintAssignable)
  FPassportImxBatchNftTransferIdOutputPin Failed;
};
