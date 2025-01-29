#pragma once

#include "ImmutableOnRampQueryParams.generated.h"

/** 
 * Represents the query parameters for generating an on-ramp URL.
 */
USTRUCT(BlueprintType)
struct IMMUTABLEMARKETPLACE_API FImmutableOnRampQueryParams
{
	GENERATED_BODY()

public:
	/** The cryptocurrency to purchase (default: "IMX"). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable|Marketplace")
	FString DefaultCryptoCurrency = TEXT("IMX");

	/** The amount of fiat currency to spend when purchasing cryptocurrency (default: "50"). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable|Marketplace")
	FString DefaultFiatAmount = TEXT("50");

	/** The fiat currency to use (default: "USD"). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable|Marketplace")
	FString DefaultFiatCurrency = TEXT("USD");

	/** A comma-separated list of available cryptocurrencies for purchase (default: "imx,eth,usdc"). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Immutable|Marketplace")
	FString CryptoCurrencyList = TEXT("imx,eth,usdc");
};