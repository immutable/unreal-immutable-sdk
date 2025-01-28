#pragma once

#include "ImmutableMarketplaceLinkConfig.generated.h"

enum class EImmutableEnvironment : uint8;

enum class EImmutableMarketplaceFlow : uint8;

/**
 * 
 */
UCLASS()
class IMMUTABLEMARKETPLACE_API UImmutableMarketplaceLinkConfig : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Immutable")
	static FString GetBaseUrl(EImmutableEnvironment Environment, EImmutableMarketplaceFlow Flow);

	UFUNCTION(BlueprintPure, Category = "Immutable")
	static FString GetApiKey(EImmutableEnvironment Environment, EImmutableMarketplaceFlow Flow);

private:
	static FString OnRampBaseUrlSandbox;
	static FString OnRampBaseUrlProduction;

	static FString SwapBaseUrlSandbox;
	static FString SwapBaseUrlProduction;

	static FString BridgeBaseUrlSandbox;
	static FString BridgeBaseUrlProduction;

	// These can be hardcoded as they are public API keys
	static FString ApiOnRampKeySandbox;
	static FString ApiOnRampKeyProduction;
};