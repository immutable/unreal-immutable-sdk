// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportConnectPKCEAsyncAction.generated.h"

UCLASS()
class IMMUTABLE_API UImtblPassportConnectPKCEAsyncAction
    : public UImtblBlueprintAsyncAction {
  GENERATED_BODY()

  DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPassportConnectPKCEOutputPin,
                                              FString, ErrorMessage);

public:
  UFUNCTION(BlueprintCallable,
            meta = (WorldContext = "WorldContextObject",
                    BlueprintInternalUseOnly = "true"),
            Category = "Immutable")
  static UImtblPassportConnectPKCEAsyncAction *
  ConnectPKCE(UObject *WorldContextObject);

  void Activate() override;

private:
  FImmutablePassportConnectData ConnectData;

  void DoConnectPKCE(TWeakObjectPtr<class UImtblJSConnector> JSConnector);
  void OnConnectPKCE(FImmutablePassportResult Result);

  UPROPERTY(BlueprintAssignable)
  FPassportConnectPKCEOutputPin Success;
  UPROPERTY(BlueprintAssignable)
  FPassportConnectPKCEOutputPin Failed;
};
