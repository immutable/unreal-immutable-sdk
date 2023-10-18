// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportGetEmailAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImtblPassportGetEmailAsyncAction
    : public UImtblBlueprintAsyncAction {
  GENERATED_BODY()

  DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPassportGetEmailOutputPin,
                                               FString, ErrorMessage, FString,
                                               Email);

public:
  UFUNCTION(BlueprintCallable,
            meta = (WorldContext = "WorldContextObject",
                    BlueprintInternalUseOnly = "true"),
            Category = "Immutable")
  static UImtblPassportGetEmailAsyncAction *
  GetEmail(UObject *WorldContextObject);

  void Activate() override;

private:
  void DoGetEmail(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
  void OnGetEmailResponse(FImmutablePassportResult Result);

  UPROPERTY(BlueprintAssignable)
  FPassportGetEmailOutputPin GotEmail;
  UPROPERTY(BlueprintAssignable)
  FPassportGetEmailOutputPin Failed;
};
