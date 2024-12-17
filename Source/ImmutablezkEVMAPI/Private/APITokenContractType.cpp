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

#include "APITokenContractType.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

inline FString ToString(const APITokenContractType::Values& Value)
{
	switch (Value)
	{
	case APITokenContractType::Values::ERC20:
		return TEXT("ERC20");
	}

	UE_LOG(LogImmutablezkEVMAPI, Error, TEXT("Invalid APITokenContractType::Values Value (%d)"), (int)Value);
	return TEXT("");
}

FString APITokenContractType::EnumToString(const APITokenContractType::Values& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, APITokenContractType::Values& Value)
{
	static TMap<FString, APITokenContractType::Values> StringToEnum = { 
		{ TEXT("ERC20"), APITokenContractType::Values::ERC20 }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool APITokenContractType::EnumFromString(const FString& EnumAsString, APITokenContractType::Values& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const APITokenContractType::Values& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, APITokenContractType::Values& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void APITokenContractType::WriteJson(JsonWriter& Writer) const
{
	WriteJsonValue(Writer, Value);
}

bool APITokenContractType::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Value);
}

}