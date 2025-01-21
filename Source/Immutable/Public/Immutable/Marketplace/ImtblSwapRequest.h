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
UCLASS(BlueprintType, Config = "Immutable", DefaultConfig)
class IMMUTABLE_API UImtblSwapRequest : public UObject
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
	 * Finds the api key associated with the provided key.
	 *
	 * @param	Environment		The environment that will be used to look the value up. Only sandbox and production environment are supported.
	 * @param	ApiKey			The api key associated with the key.
	 * @return					True if an item was found (False indicates nothing in the map uses the provided environment).
	 */
	UFUNCTION(BlueprintPure, Category = "Immutable")
	bool GetApiKey(EImtblEnvironment Environment, FString& ApiKey) const;

	/**
	 * Generates a link for the swap flow.
	 *
	 * @param	ComputedPath		[Out]		The generated link.
	 * @param	Environment			[In]		The environment for which the path is being computed for (e.g. sandbox, production).
	 * @param	ToTokenAddress		[Optional]	The token address of the token being swapped from. Default value is an empty string.
	 * @param	FromTokenAddress	[Optional]	The token address of the token being swapped to. Default value is an empty string.
	 *
	 * @return									True if path was computed (False indicates link failed to compute).
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = "False", Category = "Immutable")
	bool ComputePath(FString& ComputedPath, EImtblEnvironment Environment, FString FromTokenAddress = TEXT(""), FString ToTokenAddress = TEXT("")) const;

public:
	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Immutable")
	TMap<EImtblEnvironment, FString> BaseUrls;

	UPROPERTY(Config, EditDefaultsOnly, BlueprintReadOnly, Category = "Immutable")
	TMap<EImtblEnvironment, FString> ApiKeys;
};