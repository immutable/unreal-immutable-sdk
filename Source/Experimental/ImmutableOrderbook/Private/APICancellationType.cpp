/**
 * TS SDK API
 * running ts sdk as an api
 *
 * OpenAPI spec version: 1.0.0
 * Contact: contact@immutable.com
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * https://github.com/OpenAPITools/openapi-generator
 * Do not edit the class manually.
 */

#include "APICancellationType.h"

#include "ImmutableOrderbookModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutableOrderbook
{

inline FString ToString(const APICancellationType::Values& Value)
{
	switch (Value)
	{
	case APICancellationType::Values::OnChain:
		return TEXT("ON_CHAIN");
	case APICancellationType::Values::OffChain:
		return TEXT("OFF_CHAIN");
	case APICancellationType::Values::Underfunded:
		return TEXT("UNDERFUNDED");
	}

	UE_LOG(LogImmutableOrderbook, Error, TEXT("Invalid APICancellationType::Values Value (%d)"), (int)Value);
	return TEXT("");
}

FString APICancellationType::EnumToString(const APICancellationType::Values& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, APICancellationType::Values& Value)
{
	static TMap<FString, APICancellationType::Values> StringToEnum = { 
		{ TEXT("ON_CHAIN"), APICancellationType::Values::OnChain },
		{ TEXT("OFF_CHAIN"), APICancellationType::Values::OffChain },
		{ TEXT("UNDERFUNDED"), APICancellationType::Values::Underfunded }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool APICancellationType::EnumFromString(const FString& EnumAsString, APICancellationType::Values& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const APICancellationType::Values& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, APICancellationType::Values& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void APICancellationType::WriteJson(JsonWriter& Writer) const
{
	WriteJsonValue(Writer, Value);
}

bool APICancellationType::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Value);
}

}