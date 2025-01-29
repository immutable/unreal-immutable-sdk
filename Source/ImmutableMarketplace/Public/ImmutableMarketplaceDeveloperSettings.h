#pragma once

#include "Engine/DeveloperSettings.h"

#include "ImmutableMarketplaceDeveloperSettings.generated.h"

/**
 * Immutable Marketplace developer settings is a configuration class for the ImmutableMarketplace module.
 * This class contains settings that can be adjusted to control the behavior
 * of the ImmutableMarketplace tools within the Unreal Engine environment.
 */
UCLASS(Config = "Game", DefaultConfig)
class IMMUTABLEMARKETPLACE_API UImmutableMarketplaceDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
};