// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportGetAddressAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImtblPassportGetAddressAsyncAction
    : public UImtblBlueprintAsyncAction {
  GENERATED_BODY()

  DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPassportGetAddressOutputPin,
                                               FString, ErrorMessage, FString,
                                               Address);

public:
  UFUNCTION(BlueprintCallable,
            meta = (WorldContext = "WorldContextObject",
                    BlueprintInternalUseOnly = "true"),
            Category = "Immutable")
  static UImtblPassportGetAddressAsyncAction *
  GetAddress(UObject *WorldContextObject);

  void Activate() override;

private:
  void DoGetAddress(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
  void OnGetAddressResponse(FImmutablePassportResult Result);

  UPROPERTY(BlueprintAssignable)
  FPassportGetAddressOutputPin GotAddress;
  UPROPERTY(BlueprintAssignable)
  FPassportGetAddressOutputPin Failed;
};
