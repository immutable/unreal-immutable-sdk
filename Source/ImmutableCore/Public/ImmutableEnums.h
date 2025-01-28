#pragma once

#include "ImmutableEnums.generated.h"

UENUM(BlueprintType)
enum class EImmutableEnvironment : uint8
{
	Sandbox,
	Production,
	MAX
};

ENUM_RANGE_BY_COUNT(EImmutableEnvironment, EImmutableEnvironment::MAX)