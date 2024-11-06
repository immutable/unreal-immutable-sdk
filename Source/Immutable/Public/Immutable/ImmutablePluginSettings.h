#pragma once

#include "Engine/DeveloperSettings.h"
#include "ApplicationConfig.h"

#include "ImmutablePluginSettings.generated.h"


/**
 * 
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Immutable Plugin Settings"))
class IMMUTABLE_API UImmutablePluginSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	TSubclassOf<UApplicationConfig> DefaultApplicationConfig;

};
