// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ImtblBlueprintAsyncAction.h"

#include "ImtblPassportGetLinkedAddressesAsyncAction.generated.h"

/**
 * The Blueprint node retrieves a list of wallet addresses linked to the passport wallet.   
 */
UCLASS()
class IMMUTABLE_API UImtblPassportGetLinkedAddressesAsyncAction : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPassportGetLinkedAddressesOutputPin, FString, ErrorMessage, const TArray<FString> &, Addresses);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblPassportGetLinkedAddressesAsyncAction* GetLinkedAddresses(UObject* WorldContextObject);

	virtual void Activate() override;

private:
	void DoGetLinkedAddresses(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
	void OnGetLinkedAddressesResponse(struct FImmutablePassportResult Result);

	UPROPERTY(BlueprintAssignable)
	FPassportGetLinkedAddressesOutputPin Success;
	UPROPERTY(BlueprintAssignable)
	FPassportGetLinkedAddressesOutputPin Failed;
};
