// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportInitializationAsyncAction.generated.h"



/**
 * 
 */
UCLASS()
class IMMUTABLE_API UImtblPassportInitializationAsyncAction : public UImtblBlueprintAsyncAction
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPassportInitializationOutputPin, FString, Message);
    
public:
    UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
    static UImtblPassportInitializationAsyncAction* InitializePassport(UObject* WorldContextObject, const FString& ClientID);

    virtual void Activate() override;

private:
    FString ClientId;
    
    UPROPERTY(BlueprintAssignable)
    FPassportInitializationOutputPin Initialized;
    UPROPERTY(BlueprintAssignable)
    FPassportInitializationOutputPin Failed;

    void DoInit(TWeakObjectPtr<class UImtblJSConnector> JSConnector);
    void OnInitialized(FImmutablePassportResult Result);
};
