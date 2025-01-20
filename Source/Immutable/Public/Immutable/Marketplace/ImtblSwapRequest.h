#pragma once

#include "ImtblSwapRequest.generated.h"

enum class EImtblEnvironment : uint8;

/**
 * Swap request query params
 */
USTRUCT(BlueprintType)
struct FImtblSwapRequestQueryParams
{
	GENERATED_BODY()

public:
	FString GetPercentEncodedUrl() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable")
	FString PublishableKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable")
	FString FromTokenAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable")
	FString ToTokenAddress;
};

/**
 * Functionality to generate swap requests
 */
// UCLASS(BlueprintType, Config = "Immutable", DefaultConfig)
UCLASS(BlueprintType, Config = "Immutable")
class IMMUTABLE_API UImtblSwapRequest : public UObject
{
	GENERATED_BODY()

public:
	/** 
	 * Finds the base url associated with the environment.
	 *
	 * @param	Environment		The environment that will be used to look the value up.
	 * @param	BaseUrl			The base url associated with the key.
	 * @return					True if an item was found (False indicates nothing in the map uses the provided environment).
	 */
	UFUNCTION(BlueprintPure, Category = "Immutable")
	bool GetBaseUrl(EImtblEnvironment Environment, FString& BaseUrl) const;

	/** 
	 * Finds the api key associated with the provided key.
	 *
	 * @param	Environment		The environment that will be used to look the value up.
	 * @param	ApiKey			The api key associated with the key.
	 * @return					True if an item was found (False indicates nothing in the map uses the provided environment).
	 */
	UFUNCTION(BlueprintPure, Category = "Immutable")
	bool GetApiKey(EImtblEnvironment Environment, FString& ApiKey) const;

	/**
	 * Generates a path link for the swap flow.
	 *
	 * @param	Environment			The environment that will be used to compute the path.
	 * @param	ToTokenAddress		The address of the token being swapped from. Also known as the token contract address.
	 * @param	FromTokenAddress	The address of the token being swapped to. Also known as the token contract address.
	 * @param	ComputedPath [OUT]	The address of the token being swapped to. Also known as the token contract address.
	 * @return						True if path was computed (False indicates path failed to compute).
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = "False", Category = "Immutable")
	bool ComputePath(EImtblEnvironment Environment, FString FromTokenAddress, FString ToTokenAddress, FString& ComputedPath) const;

public:
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Immutable")
	TMap<EImtblEnvironment, FString> BaseUrls;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Immutable")
	TMap<EImtblEnvironment, FString> ApiKeys;
};