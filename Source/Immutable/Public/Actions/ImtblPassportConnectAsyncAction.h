// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportConnectAsyncAction.generated.h"


/**
 * 
 */
UCLASS()
class IMMUTABLE_API UImtblPassportConnectAsyncAction : public UImtblBlueprintAsyncAction
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPassportConnectOutputPin, FString, ErrorMessage);
    
public:
    UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
    static UImtblPassportConnectAsyncAction* Connect(UObject* WorldContextObject);

    virtual void Activate() override;

private:
    FImmutablePassportConnectData ConnectData;
    
    void DoConnect(TWeakObjectPtr<class UImtblJSConnector> JSConnector);
    void OnConnect(FImmutablePassportResult Result);

    UPROPERTY(BlueprintAssignable)
    FPassportConnectOutputPin Success;
    UPROPERTY(BlueprintAssignable)
    FPassportConnectOutputPin Failed;
};
