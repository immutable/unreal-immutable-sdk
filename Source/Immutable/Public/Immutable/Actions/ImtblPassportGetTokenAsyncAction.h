// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ImtblBlueprintAsyncAction.h"

#include "ImtblPassportGetTokenAsyncAction.generated.h"


/**
 * Blueprint Node Utility to get tokens such as Id and Access tokens 
 */
UCLASS()
class IMMUTABLE_API UImtblPassportGetTokenAsyncAction : public UImtblBlueprintAsyncAction
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPassportTokenOutputPin, FString, ErrorMessage, FString, Token);

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblPassportGetTokenAsyncAction* GetAccessToken(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblPassportGetTokenAsyncAction* GetIdToken(UObject* WorldContextObject);

	virtual void Activate() override;

private:
	enum TokenType
	{
		ID,
		ACCESS,
		NONE
	};
	
	void DoGetToken(TWeakObjectPtr<class UImtblJSConnector> JSGetConnector);
	void OnGetTokenResponse(struct FImmutablePassportResult Result);

	UPROPERTY(BlueprintAssignable)
	FPassportTokenOutputPin Success;
	UPROPERTY(BlueprintAssignable)
	FPassportTokenOutputPin Failed;

	TokenType Type = NONE;
};
