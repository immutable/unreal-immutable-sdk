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

#include "APIAPIError401.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

inline FString ToString(const APIAPIError401::CodeEnum& Value)
{
	switch (Value)
	{
	case APIAPIError401::CodeEnum::UnauthorisedRequest:
		return TEXT("UNAUTHORISED_REQUEST");
	}

	UE_LOG(LogImmutablezkEVMAPI, Error, TEXT("Invalid APIAPIError401::CodeEnum Value (%d)"), (int)Value);
	return TEXT("");
}

FString APIAPIError401::EnumToString(const APIAPIError401::CodeEnum& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, APIAPIError401::CodeEnum& Value)
{
	static TMap<FString, APIAPIError401::CodeEnum> StringToEnum = { 
		{ TEXT("UNAUTHORISED_REQUEST"), APIAPIError401::CodeEnum::UnauthorisedRequest }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool APIAPIError401::EnumFromString(const FString& EnumAsString, APIAPIError401::CodeEnum& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const APIAPIError401::CodeEnum& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, APIAPIError401::CodeEnum& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void APIAPIError401::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("message")); WriteJsonValue(Writer, Message);
	Writer->WriteIdentifierPrefix(TEXT("link")); WriteJsonValue(Writer, Link);
	Writer->WriteIdentifierPrefix(TEXT("trace_id")); WriteJsonValue(Writer, TraceId);
	Writer->WriteIdentifierPrefix(TEXT("code")); WriteJsonValue(Writer, Code);
	if (Details.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("details")); WriteJsonValue(Writer, Details.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool APIAPIError401::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("message"), Message);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("link"), Link);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("trace_id"), TraceId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("code"), Code);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("details"), Details);

	return ParseSuccess;
}

}
