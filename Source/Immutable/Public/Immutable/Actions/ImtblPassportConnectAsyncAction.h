// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportConnectAsyncAction.generated.h"


/**
 * 
 */
UCLASS()
class IMMUTABLE_API UImtblPassportConnectAsyncAction : public UImtblBlueprintAsyncAction
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FPassportConnectOutputPin, FString, ErrorMessage, FString, Code, FString, DeviceCode, FString, Url);
    
public:
    UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
    static UImtblPassportConnectAsyncAction* Connect(UObject* WorldContextObject);

    virtual void Activate() override;

private:
    FImmutablePassportConnectData ConnectData;
    
    void DoConnect(TWeakObjectPtr<class UImtblJSConnector> JSConnector);
    void OnConnectCodeReady(FImmutablePassportResult Result);

    UPROPERTY(BlueprintAssignable)
    FPassportConnectOutputPin CodeReady;
    UPROPERTY(BlueprintAssignable)
    FPassportConnectOutputPin Failed;
};
