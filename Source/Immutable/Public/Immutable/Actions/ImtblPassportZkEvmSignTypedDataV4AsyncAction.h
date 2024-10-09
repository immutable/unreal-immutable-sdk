// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportZkEvmSignTypedDataV4AsyncAction.generated.h"

/**
 * Async action blueprint node for zkEVM Send Transaction
 */
UCLASS()
class IMMUTABLE_API UImtblPassportZkEvmSignTypedDataV4AsyncAction : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPassportZkEvmSignTypedDataV4OutputPin, FString, ErrorMessage, FString, Signature);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblPassportZkEvmSignTypedDataV4AsyncAction* ZkEvmSignTypedDataV4(UObject* WorldContextObject, const FZkEvmSignTypedDataV4Request& Request);

	virtual void Activate() override;

private:
	FZkEvmSignTypedDataV4Request SignRequest;

	UPROPERTY(BlueprintAssignable)
	FPassportZkEvmSignTypedDataV4OutputPin MessageSigned;
	UPROPERTY(BlueprintAssignable)
	FPassportZkEvmSignTypedDataV4OutputPin Failed;

	void DoZkEvmSignTypedDataV4(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
	void OnZkEvmSignTypedDataV4Response(FImmutablePassportResult Result);
};
