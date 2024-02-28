// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Immutable/ImmutablePassport.h"
#include "ImtblBlueprintAsyncAction.h"
#include "ImtblPassportGetIdTokenAsyncAction.generated.h"

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImtblPassportGetIdTokenAsyncAction : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPassportGetIdTokenOutputPin, FString, ErrorMessage, FString, IdToken);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblPassportGetIdTokenAsyncAction* GetIdToken(UObject* WorldContextObject);

	virtual void Activate() override;

private:
	void DoGetIdToken(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
	void OnGetIdTokenResponse(FImmutablePassportResult Result);

	UPROPERTY(BlueprintAssignable)
	FPassportGetIdTokenOutputPin GotIdToken;
	UPROPERTY(BlueprintAssignable)
	FPassportGetIdTokenOutputPin Failed;
};
