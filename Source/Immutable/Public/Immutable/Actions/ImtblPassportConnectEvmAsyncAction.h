// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportConnectEvmAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImtblPassportConnectEvmAsyncAction : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPassportConnectEvmOutputPin, FString, ErrorMessage);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblPassportConnectEvmAsyncAction* ConnectEvm(UObject* WorldContextObject);

	virtual void Activate() override;

private:
	void DoConnectEvm(TWeakObjectPtr<class UImtblJSConnector> JSConnector);
	void OnConnectEvm(FImmutablePassportResult Result);

	UPROPERTY(BlueprintAssignable)
	FPassportConnectEvmOutputPin Success;
	UPROPERTY(BlueprintAssignable)
	FPassportConnectEvmOutputPin Failed;
};
