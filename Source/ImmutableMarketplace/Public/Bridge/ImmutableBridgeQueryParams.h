﻿#pragma once

#include "ImmutableBridgeQueryParams.generated.h"

/**
 * Represents the query parameters for generating a bridge URL.
 */
USTRUCT(BlueprintType)
struct IMMUTABLEMARKETPLACE_API FImmutableBridgeQueryParams
{
	GENERATED_BODY()

public:
	/** The ID of the source blockchain (default is null). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable|Marketplace")
	FString FromChainID;

	/** The address of the token being moved from (default is null). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable|Marketplace")
	FString FromTokenAddress;

	/** The ID of the destination blockchain (default is null). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable|Marketplace")
	FString ToChainID;

	/** The address of the token being moved to (default is null). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable|Marketplace")
	FString ToTokenAddress;
};