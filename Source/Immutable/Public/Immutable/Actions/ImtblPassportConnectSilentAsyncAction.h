// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportConnectSilentAsyncAction.generated.h"


/**
 * 
 */
UCLASS()
class IMMUTABLE_API UImtblPassportConnectSilentAsyncAction : public UImtblBlueprintAsyncAction
{
    GENERATED_BODY()
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPassportConnectSilentOutputPin, FString, ErrorMessage);
    
public:
    UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
    static UImtblPassportConnectSilentAsyncAction* ConnectSilent(UObject* WorldContextObject);

    virtual void Activate() override;

private:
    FImmutablePassportConnectData ConnectData;
    
    UPROPERTY(BlueprintAssignable)
    FPassportConnectSilentOutputPin Connected;
    UPROPERTY(BlueprintAssignable)
    FPassportConnectSilentOutputPin Failed;
    
    void DoConnectSilent(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
    void OnConnectSilentResponse(FImmutablePassportResult Result);

};
