/**
 * Immutable zkEVM API
 * Immutable Multi Rollup API
 *
 * OpenAPI spec version: 1.0.0
 * Contact: support@immutable.com
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * https://github.com/OpenAPITools/openapi-generator
 * Do not edit the class manually.
 */

#include "APIOperatorAllowlistStatus.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

inline FString ToString(const APIOperatorAllowlistStatus::Values& Value)
{
	switch (Value)
	{
	case APIOperatorAllowlistStatus::Values::Requested:
		return TEXT("requested");
	case APIOperatorAllowlistStatus::Values::Approved:
		return TEXT("approved");
	case APIOperatorAllowlistStatus::Values::Rejected:
		return TEXT("rejected");
	case APIOperatorAllowlistStatus::Values::Removed:
		return TEXT("removed");
	case APIOperatorAllowlistStatus::Values::Added:
		return TEXT("added");
	}

	UE_LOG(LogImmutablezkEVMAPI, Error, TEXT("Invalid APIOperatorAllowlistStatus::Values Value (%d)"), (int)Value);
	return TEXT("");
}

FString APIOperatorAllowlistStatus::EnumToString(const APIOperatorAllowlistStatus::Values& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, APIOperatorAllowlistStatus::Values& Value)
{
	static TMap<FString, APIOperatorAllowlistStatus::Values> StringToEnum = { 
		{ TEXT("requested"), APIOperatorAllowlistStatus::Values::Requested },
		{ TEXT("approved"), APIOperatorAllowlistStatus::Values::Approved },
		{ TEXT("rejected"), APIOperatorAllowlistStatus::Values::Rejected },
		{ TEXT("removed"), APIOperatorAllowlistStatus::Values::Removed },
		{ TEXT("added"), APIOperatorAllowlistStatus::Values::Added }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool APIOperatorAllowlistStatus::EnumFromString(const FString& EnumAsString, APIOperatorAllowlistStatus::Values& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const APIOperatorAllowlistStatus::Values& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, APIOperatorAllowlistStatus::Values& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void APIOperatorAllowlistStatus::WriteJson(JsonWriter& Writer) const
{
	WriteJsonValue(Writer, Value);
}

bool APIOperatorAllowlistStatus::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Value);
}

}
