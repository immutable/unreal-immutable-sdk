#pragma once

UENUM(BlueprintType)
enum class EImtblEnvironment : uint8
{
	Sandbox,
	Production,
	MAX
};

ENUM_RANGE_BY_COUNT(EImtblEnvironment, EImtblEnvironment::MAX)