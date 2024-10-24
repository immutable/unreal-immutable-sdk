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

#include "APIFeeType.h"

#include "ImmutableOrderbookModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutableOrderbook
{

inline FString ToString(const APIFeeType::Values& Value)
{
	switch (Value)
	{
	case APIFeeType::Values::MakerEcosystem:
		return TEXT("MAKER_ECOSYSTEM");
	case APIFeeType::Values::TakerEcosystem:
		return TEXT("TAKER_ECOSYSTEM");
	case APIFeeType::Values::Protocol:
		return TEXT("PROTOCOL");
	case APIFeeType::Values::Royalty:
		return TEXT("ROYALTY");
	}

	UE_LOG(LogImmutableOrderbook, Error, TEXT("Invalid APIFeeType::Values Value (%d)"), (int)Value);
	return TEXT("");
}

FString APIFeeType::EnumToString(const APIFeeType::Values& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, APIFeeType::Values& Value)
{
	static TMap<FString, APIFeeType::Values> StringToEnum = { 
		{ TEXT("MAKER_ECOSYSTEM"), APIFeeType::Values::MakerEcosystem },
		{ TEXT("TAKER_ECOSYSTEM"), APIFeeType::Values::TakerEcosystem },
		{ TEXT("PROTOCOL"), APIFeeType::Values::Protocol },
		{ TEXT("ROYALTY"), APIFeeType::Values::Royalty }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool APIFeeType::EnumFromString(const FString& EnumAsString, APIFeeType::Values& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const APIFeeType::Values& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, APIFeeType::Values& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void APIFeeType::WriteJson(JsonWriter& Writer) const
{
	WriteJsonValue(Writer, Value);
}

bool APIFeeType::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Value);
}

}
