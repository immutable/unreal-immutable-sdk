// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportHasCredentialsSavedAsyncAction.generated.h"



/**
 * 
 */
UCLASS()
class IMMUTABLE_API UImtblPassportHasCredentialsSavedAsyncAction : public UImtblBlueprintAsyncAction
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPassportHasCredentialsSavedOutputPin, FString, ErrorMessage, FString, Credentials);
    
public:
    UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
    static UImtblPassportHasCredentialsSavedAsyncAction* HasCredentialsSaved(UObject* WorldContextObject);

    virtual void Activate() override;

private:
    void DoHasCredentialsSaved(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
    void OnHasCredentialsSavedResponse(FImmutablePassportResult Result);

    UPROPERTY(BlueprintAssignable)
    FPassportHasCredentialsSavedOutputPin HasCredentials;
    UPROPERTY(BlueprintAssignable)
    FPassportHasCredentialsSavedOutputPin Failed;
};
