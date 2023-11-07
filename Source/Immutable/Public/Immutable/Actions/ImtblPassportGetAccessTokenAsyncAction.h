// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportGetAccessTokenAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImtblPassportGetAccessTokenAsyncAction
    : public UImtblBlueprintAsyncAction {
  GENERATED_BODY()

  DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPassportGetAccessTokenOutputPin,
                                               FString, ErrorMessage, FString,
                                               AccessToken);

public:
  UFUNCTION(BlueprintCallable,
            meta = (WorldContext = "WorldContextObject",
                    BlueprintInternalUseOnly = "true"),
            Category = "Immutable")
  static UImtblPassportGetAccessTokenAsyncAction *
  GetAccessToken(UObject *WorldContextObject);

  void Activate() override;

private:
  void DoGetAccessToken(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
  void OnGetAccessTokenResponse(FImmutablePassportResult Result);

  UPROPERTY(BlueprintAssignable)
  FPassportGetAccessTokenOutputPin GotAccessToken;
  UPROPERTY(BlueprintAssignable)
  FPassportGetAccessTokenOutputPin Failed;
};
