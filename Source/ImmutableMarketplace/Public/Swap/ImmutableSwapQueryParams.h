#pragma once

#include "ImmutableSwapQueryParams.generated.h"

/**
 * Represents the query parameters for generating a swap URL. 
 */
USTRUCT(BlueprintType)
struct IMMUTABLEMARKETPLACE_API FImmutableSwapQueryParams
{
	GENERATED_BODY()

public:
	/** The address of the token being swapped from (default: ""). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable|Marketplace")
	FString FromTokenAddress;

	/** The address of the token being swapped to (default: ""). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable|Marketplace")
	FString ToTokenAddress;
};