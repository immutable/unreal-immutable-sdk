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

#include "APIAction.h"

#include "ImmutableOrderbookModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutableOrderbook
{

inline FString ToString(const APIAction::TypeEnum& Value)
{
	switch (Value)
	{
	case APIAction::TypeEnum::Transaction:
		return TEXT("TRANSACTION");
	case APIAction::TypeEnum::Signable:
		return TEXT("SIGNABLE");
	}

	UE_LOG(LogImmutableOrderbook, Error, TEXT("Invalid APIAction::TypeEnum Value (%d)"), (int)Value);
	return TEXT("");
}

FString APIAction::EnumToString(const APIAction::TypeEnum& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, APIAction::TypeEnum& Value)
{
	static TMap<FString, APIAction::TypeEnum> StringToEnum = { 
		{ TEXT("TRANSACTION"), APIAction::TypeEnum::Transaction },
		{ TEXT("SIGNABLE"), APIAction::TypeEnum::Signable }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool APIAction::EnumFromString(const FString& EnumAsString, APIAction::TypeEnum& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const APIAction::TypeEnum& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, APIAction::TypeEnum& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void APIAction::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	if (PopulatedTransactions.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("populatedTransactions")); WriteJsonValue(Writer, PopulatedTransactions.GetValue());
	}
	if (Purpose.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("purpose")); WriteJsonValue(Writer, Purpose.GetValue());
	}
	Writer->WriteIdentifierPrefix(TEXT("type")); WriteJsonValue(Writer, Type);
	if (Message.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("message")); WriteJsonValue(Writer, Message.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool APIAction::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("populatedTransactions"), PopulatedTransactions);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("purpose"), Purpose);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("type"), Type);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("message"), Message);

	return ParseSuccess;
}

}
