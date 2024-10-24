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

#include "APIOrderStatusOneOf5.h"

#include "ImmutableOrderbookModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutableOrderbook
{

inline FString ToString(const APIOrderStatusOneOf5::NameEnum& Value)
{
	switch (Value)
	{
	case APIOrderStatusOneOf5::NameEnum::Pending:
		return TEXT("PENDING");
	}

	UE_LOG(LogImmutableOrderbook, Error, TEXT("Invalid APIOrderStatusOneOf5::NameEnum Value (%d)"), (int)Value);
	return TEXT("");
}

FString APIOrderStatusOneOf5::EnumToString(const APIOrderStatusOneOf5::NameEnum& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, APIOrderStatusOneOf5::NameEnum& Value)
{
	static TMap<FString, APIOrderStatusOneOf5::NameEnum> StringToEnum = { 
		{ TEXT("PENDING"), APIOrderStatusOneOf5::NameEnum::Pending }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool APIOrderStatusOneOf5::EnumFromString(const FString& EnumAsString, APIOrderStatusOneOf5::NameEnum& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const APIOrderStatusOneOf5::NameEnum& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, APIOrderStatusOneOf5::NameEnum& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void APIOrderStatusOneOf5::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	if (Evaluated.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("evaluated")); WriteJsonValue(Writer, Evaluated.GetValue());
	}
	if (Name.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("name")); WriteJsonValue(Writer, Name.GetValue());
	}
	if (Started.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("started")); WriteJsonValue(Writer, Started.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool APIOrderStatusOneOf5::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("evaluated"), Evaluated);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("name"), Name);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("started"), Started);

	return ParseSuccess;
}

}
