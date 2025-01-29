#pragma once

#include "ImmutableMarketplaceEnums.generated.h"

UENUM(BlueprintType)
enum class EImmutableMarketplaceFlow : uint8
{
	OnRamp,
	Swap,
	Bridge
};