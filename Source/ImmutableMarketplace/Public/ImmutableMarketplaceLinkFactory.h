#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "Bridge/ImmutableBridgeQueryParams.h"
#include "OnRamp/ImmutableOnRampQueryParams.h"
#include "Swap/ImmutableSwapQueryParams.h"

#include "ImmutableMarketplaceLinkFactory.generated.h"

enum class EImmutableEnvironment : uint8;

UCLASS()
class IMMUTABLEMARKETPLACE_API UImmutableMarketplaceLinkFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Generates a URL for the on-ramp flow.
	 *
	 * @param Environment		Specifies the environment (Sandbox or Production).
	 * @param Email				The user's email address, pre-filled in the on-ramp flow.
	 * @param WalletAddress		The user's wallet address, where tokens will be sent.
	 * @param QueryParams		The query parameters for the on-ramp flow. Uses default values if not specified.
	 * @param ExtraQueryParams	Additional query parameters. See https://docs.transak.com/docs/query-parameters for possible fields.
	 *							If ExtraQueryParams includes any fields that are already defined in QueryParams, the values in QueryParams will take precedence.
	 *							For example, if ExtraQueryParams contains "defaultFiatAmount", it will be ignored and the value from QueryParams.DefaultFiatAmount will be used instead.
	 *
	 * @return					The generated on-ramp URL.
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|Marketplace")
	static FString GenerateOnRampLink(EImmutableEnvironment Environment, FString Email, FString WalletAddress, const FImmutableOnRampQueryParams& QueryParams, TMap<FString, FString> ExtraQueryParams);

	/**
	 * Generates a link for the swap flow.
	 *
	 * @param Environment		Specifies the environment (Sandbox or Production).
	 * @param PublishableKey	The publishable key obtained from https://hub.immutable.com/. See https://docs.immutable.com/api/zkEVM/apikeys for more details.
	 * @param QueryParams		The query parameters for the swap flow. Uses default values if not specified.
	 *
	 * @return					The generated swap URL.
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|Marketplace")
	static FString GenerateSwapLink(EImmutableEnvironment Environment, FString PublishableKey, const FImmutableSwapQueryParams& QueryParams);

	/**
	 * Generates a link for the bridge flow.
	 *
	 * @param Environment	Specifies the environment (Sandbox or Production).
	 * @param QueryParams	The query parameters for the bridge flow. Uses default values if not specified.
	 *
	 * @return				The generated bridge URL.
	 */
	UFUNCTION(BlueprintCallable, Category = "Immutable|Marketplace")
	static FString GenerateBridgeLink(EImmutableEnvironment Environment, const FImmutableBridgeQueryParams& QueryParams);
};