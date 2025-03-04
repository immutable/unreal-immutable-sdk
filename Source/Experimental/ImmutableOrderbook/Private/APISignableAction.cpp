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

#include "APISignableAction.h"

#include "ImmutableOrderbookModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutableOrderbook
{

inline FString ToString(const APISignableAction::TypeEnum& Value)
{
	switch (Value)
	{
	case APISignableAction::TypeEnum::Signable:
		return TEXT("SIGNABLE");
	}

	UE_LOG(LogImmutableOrderbook, Error, TEXT("Invalid APISignableAction::TypeEnum Value (%d)"), (int)Value);
	return TEXT("");
}

FString APISignableAction::EnumToString(const APISignableAction::TypeEnum& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, APISignableAction::TypeEnum& Value)
{
	static TMap<FString, APISignableAction::TypeEnum> StringToEnum = { 
		{ TEXT("SIGNABLE"), APISignableAction::TypeEnum::Signable }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool APISignableAction::EnumFromString(const FString& EnumAsString, APISignableAction::TypeEnum& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const APISignableAction::TypeEnum& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, APISignableAction::TypeEnum& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void APISignableAction::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	if (Message.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("message")); WriteJsonValue(Writer, Message.GetValue());
	}
	if (Purpose.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("purpose")); WriteJsonValue(Writer, Purpose.GetValue());
	}
	Writer->WriteIdentifierPrefix(TEXT("type")); WriteJsonValue(Writer, Type);
	Writer->WriteObjectEnd();
}

bool APISignableAction::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("message"), Message);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("purpose"), Purpose);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("type"), Type);

	return ParseSuccess;
}

}
