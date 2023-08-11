// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportConfirmCodeAsyncAction.generated.h"



/**
 * 
 */
UCLASS()
class IMMUTABLE_API UImtblPassportConfirmCodeAsyncAction : public UImtblBlueprintAsyncAction
{
    GENERATED_BODY()

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPassportConfirmCodeOutputPin, FString, ErrorMessage);
    
public:
    UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
    static UImtblPassportConfirmCodeAsyncAction* ConfirmCode(UObject* WorldContextObject, const FString& DeviceCode);

    virtual void Activate() override;

    void DoConfirmCode(TWeakObjectPtr<class UImtblJSConnector> JSConnector);
    void OnConfirmCodeCodeResponse(FImmutablePassportResult Result);

    UPROPERTY(BlueprintAssignable)
    FPassportConfirmCodeOutputPin CodeConfirmed;
    UPROPERTY(BlueprintAssignable)
    FPassportConfirmCodeOutputPin Failed;

private:
    FString DeviceCode;
};
