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
	/**
	 * Log into Passport
	 *
	 * @param	WorldContextObject	World context
	 *
	 * @return	A reference to the object represented by this node
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblConnectionAsyncActions* Login(UObject* WorldContextObject);

	/**
	 * Log into Passport, initialise the gamer's wallet and instantiate the IMX provider.
	 *
	 * @param	WorldContextObject	World context
	 *
	 * @return	A reference to the object represented by this node
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"), Category = "Immutable")
	static UImtblConnectionAsyncActions* ConnectImx(UObject* WorldContextObject);


	virtual void Activate() override;

private:

	void DoConnect(TWeakObjectPtr<class UImtblJSConnector> JSConnector);
	void OnConnect(FImmutablePassportResult Result);

	UPROPERTY(BlueprintAssignable)
	FPassportConnectOutputPin Success;
	UPROPERTY(BlueprintAssignable)
	FPassportConnectOutputPin Failed;

	bool bUseCachedSession = false;
	bool bIsConnectImx = false;
	bool bIsPKCE = false;
};
