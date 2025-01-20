#pragma once

#include "Engine/DeveloperSettings.h"
#include "ApplicationConfig.h"

#include "ImmutableSettings.generated.h"


/**
 * Immutable developer settings is a configuration class for the Immutable plugin.
 * This class contains settings that can be adjusted to control the behavior
 * of the Immutable plugin within the Unreal Engine environment.
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Immutable"))
class IMMUTABLE_API UImmutableSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/// The default application configuration class.
	/// This property holds a reference to a subclass of UApplicationConfig,
	/// which will be used as the default configuration for the application.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	TSubclassOf<UApplicationConfig> DefaultApplicationConfig;

};
