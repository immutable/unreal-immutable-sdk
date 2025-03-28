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

#include "APIOrderStatusOneOf1.h"

#include "ImmutableOrderbookModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutableOrderbook
{

inline FString ToString(const APIOrderStatusOneOf1::NameEnum& Value)
{
	switch (Value)
	{
	case APIOrderStatusOneOf1::NameEnum::Cancelled:
		return TEXT("CANCELLED");
	}

	UE_LOG(LogImmutableOrderbook, Error, TEXT("Invalid APIOrderStatusOneOf1::NameEnum Value (%d)"), (int)Value);
	return TEXT("");
}

FString APIOrderStatusOneOf1::EnumToString(const APIOrderStatusOneOf1::NameEnum& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, APIOrderStatusOneOf1::NameEnum& Value)
{
	static TMap<FString, APIOrderStatusOneOf1::NameEnum> StringToEnum = { 
		{ TEXT("CANCELLED"), APIOrderStatusOneOf1::NameEnum::Cancelled }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool APIOrderStatusOneOf1::EnumFromString(const FString& EnumAsString, APIOrderStatusOneOf1::NameEnum& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const APIOrderStatusOneOf1::NameEnum& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, APIOrderStatusOneOf1::NameEnum& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void APIOrderStatusOneOf1::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	if (CancellationType.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("cancellation_type")); WriteJsonValue(Writer, CancellationType.GetValue());
	}
	if (Name.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("name")); WriteJsonValue(Writer, Name.GetValue());
	}
	if (Pending.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("pending")); WriteJsonValue(Writer, Pending.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool APIOrderStatusOneOf1::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("cancellation_type"), CancellationType);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("name"), Name);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("pending"), Pending);

	return ParseSuccess;
}

}
