#pragma once

#include "Engine/DeveloperSettings.h"
#include "OnRampConfig.h"

#include "ImmutableMarketplaceSettings.generated.h"


/**
 * Immutable Marketplace developer settings is a configuration class for the ImmutableMarketplace module.
 * This class contains settings that can be adjusted to control the behavior
 * of the ImmutableMarketplace tools within the Unreal Engine environment.
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Immutable Marketplace Settings"))
class IMMUTABLEMARKETPLACE_API UImmutableMarketplaceSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/// The default on ramp widget configuration class.
	/// This property holds a reference to a subclass of UTransakConfig,
	/// which is used to load on ramp widget in web browser.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Transak")
	TSubclassOf<UTransakConfig> DefaultOnRampWidgetConfig;
};
