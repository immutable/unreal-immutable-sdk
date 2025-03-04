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

#include "APIMarketPriceNativeToken.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

inline FString ToString(const APIMarketPriceNativeToken::TypeEnum& Value)
{
	switch (Value)
	{
	case APIMarketPriceNativeToken::TypeEnum::Native:
		return TEXT("NATIVE");
	}

	UE_LOG(LogImmutablezkEVMAPI, Error, TEXT("Invalid APIMarketPriceNativeToken::TypeEnum Value (%d)"), (int)Value);
	return TEXT("");
}

FString APIMarketPriceNativeToken::EnumToString(const APIMarketPriceNativeToken::TypeEnum& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, APIMarketPriceNativeToken::TypeEnum& Value)
{
	static TMap<FString, APIMarketPriceNativeToken::TypeEnum> StringToEnum = { 
		{ TEXT("NATIVE"), APIMarketPriceNativeToken::TypeEnum::Native }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool APIMarketPriceNativeToken::EnumFromString(const FString& EnumAsString, APIMarketPriceNativeToken::TypeEnum& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const APIMarketPriceNativeToken::TypeEnum& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, APIMarketPriceNativeToken::TypeEnum& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void APIMarketPriceNativeToken::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("type")); WriteJsonValue(Writer, Type);
	if (Symbol.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("symbol")); WriteJsonValue(Writer, Symbol.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool APIMarketPriceNativeToken::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("type"), Type);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("symbol"), Symbol);

	return ParseSuccess;
}

}
