// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportHasStoredCredentialsAsyncAction.generated.h"


/**
 * 
 */
UCLASS()
class IMMUTABLE_API UImtblPassportHasStoredCredentialsAsyncAction : public UImtblBlueprintAsyncAction
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPassportHasStoredCredentialsOutputPin, FString, ErrorMessage);
    
public:
    UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
    static UImtblPassportHasStoredCredentialsAsyncAction* HasStoredCredentials(UObject* WorldContextObject);

    virtual void Activate() override;

private:
    void DoHasStoredCredentials(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
    void OnHasStoredCredentialsResponse(FImmutablePassportResult Result);

    UPROPERTY(BlueprintAssignable)
    FPassportHasStoredCredentialsOutputPin HasCredentials;
    UPROPERTY(BlueprintAssignable)
    FPassportHasStoredCredentialsOutputPin NotFound;
};
