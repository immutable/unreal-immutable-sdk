// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblConnectImxAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImtblConnectionAsyncActions : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPassportConnectOutputPin, FString, ErrorMessage);

public:
	
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblConnectionAsyncActions* Login(UObject* WorldContextObject, bool WithWalletImx = false, bool TryToRelogin = false);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblConnectionAsyncActions* LoginPKCE(UObject* WorldContextObject, bool WithWalletImx);

	void Activate() override;

private:

	FImmutablePassportInitDeviceFlowData InitDeviceFlowData;

	void DoConnect(TWeakObjectPtr<class UImtblJSConnector> JSConnector);
	void OnConnect(FImmutablePassportResult Result);

	UPROPERTY(BlueprintAssignable)
	FPassportConnectOutputPin Success;
	UPROPERTY(BlueprintAssignable)
	FPassportConnectOutputPin Failed;

	bool bIsRelogin = false;
	bool bWithWalletImx = false;
	bool bIsPKCE = false;
};
