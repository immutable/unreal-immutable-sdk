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

#include "APIPendingOrderStatus.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

inline FString ToString(const APIPendingOrderStatus::NameEnum& Value)
{
	switch (Value)
	{
	case APIPendingOrderStatus::NameEnum::Pending:
		return TEXT("PENDING");
	}

	UE_LOG(LogImmutablezkEVMAPI, Error, TEXT("Invalid APIPendingOrderStatus::NameEnum Value (%d)"), (int)Value);
	return TEXT("");
}

FString APIPendingOrderStatus::EnumToString(const APIPendingOrderStatus::NameEnum& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, APIPendingOrderStatus::NameEnum& Value)
{
	static TMap<FString, APIPendingOrderStatus::NameEnum> StringToEnum = { 
		{ TEXT("PENDING"), APIPendingOrderStatus::NameEnum::Pending }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool APIPendingOrderStatus::EnumFromString(const FString& EnumAsString, APIPendingOrderStatus::NameEnum& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const APIPendingOrderStatus::NameEnum& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, APIPendingOrderStatus::NameEnum& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void APIPendingOrderStatus::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("name")); WriteJsonValue(Writer, Name);
	Writer->WriteIdentifierPrefix(TEXT("evaluated")); WriteJsonValue(Writer, Evaluated);
	Writer->WriteIdentifierPrefix(TEXT("started")); WriteJsonValue(Writer, Started);
	Writer->WriteObjectEnd();
}

bool APIPendingOrderStatus::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("name"), Name);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("evaluated"), Evaluated);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("started"), Started);

	return ParseSuccess;
}

}
