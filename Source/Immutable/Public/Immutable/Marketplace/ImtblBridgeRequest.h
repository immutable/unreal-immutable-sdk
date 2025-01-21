#pragma once

#include "ImtblBridgeRequest.generated.h"

/**
 * Bridge request query params
 */
USTRUCT(BlueprintType)
struct FImtblBridgeRequestQueryParams
{
	GENERATED_BODY()

public:
	FString GetPercentEncodedUrl() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable")
	FString FromChainID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable")
	FString FromTokenAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable")
	FString ToChainID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable")
	FString ToTokenAddress;
};

/**
 * Functionality to generate bridge requests
 */
UCLASS(BlueprintType, Config = "Immutable", DefaultConfig)
class IMMUTABLE_API UImtblBridgeRequest : public UObject
{
	GENERATED_BODY()

public:
	/** 
	 * Finds the base url associated with the environment.
	 *
	 * @param	Environment		The environment that will be used to look the value up. Only sandbox and production environment are supported.
	 * @param	BaseUrl			The base url associated with the key.
	 * @return					True if an item was found (False indicates nothing in the map uses the provided environment).
	 */
	UFUNCTION(BlueprintPure, Category = "Immutable")
	bool GetBaseUrl(EImtblEnvironment Environment, FString& BaseUrl) const;

	/**
	 * Generates a link for the bridge flow.
	 *
	 * @param	ComputedPath		[Out]		The generated link.
	 * @param	Environment			[In]		The environment for which the path is being computed for (e.g. sandbox, production).
	 * @param	FromChainID			[Optional]	The ID of the source blockchain. Default value is an empty string.
	 * @param	FromTokenAddress	[Optional]	The contract address of the token being bridged from.
	 *											This should be a contract address of a token on the source blockchain.
	 *											Default value is an empty string.
	 * @param	ToChainID			[Optional]	The ID of the destination blockchain. Default value is an empty string.
	 * @param	ToTokenAddress		[Optional]	The contract address of the token being bridged to.
	 *											This should be a contract address of a token on the destination blockchain.
	 *											Default is an empty string.
	 *
	 * @return									True if link was computed (False indicates link failed to compute).
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = "False", Category = "Immutable")
	bool ComputePath(FString& ComputedPath, EImtblEnvironment Environment, FString FromChainID = TEXT(""), FString FromTokenAddress = TEXT(""), FString ToChainID = TEXT(""), FString ToTokenAddress = TEXT("")) const;

public:
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Immutable")
	TMap<EImtblEnvironment, FString> BaseUrls;
};