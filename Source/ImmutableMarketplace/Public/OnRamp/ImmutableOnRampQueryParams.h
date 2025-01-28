#pragma once

#include "ImmutableOnRampQueryParams.generated.h"

/** 
 * Represents the query parameters for generating an on-ramp URL.
 */
USTRUCT(BlueprintType)
struct IMMUTABLEMARKETPLACE_API FImmutableOnRampQueryParams
{
	GENERATED_BODY()

	/** The cryptocurrency to purchase (default: "IMX"). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable|Marketplace")
	FString DefaultCryptoCurrency;

	/** The amount of fiat currency to spend when purchasing cryptocurrency (default: "50"). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable|Marketplace")
	FString DefaultFiatAmount;

	/** The fiat currency to use (default: "USD"). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable|Marketplace")
	FString DefaultFiatCurrency;

	/** A comma-separated list of available cryptocurrencies for purchase (default: "imx,eth,usdc"). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable|Marketplace")
	FString CryptoCurrencyList;
};