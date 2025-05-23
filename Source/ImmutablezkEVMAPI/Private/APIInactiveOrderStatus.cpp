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

#include "APIInactiveOrderStatus.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

inline FString ToString(const APIInactiveOrderStatus::NameEnum& Value)
{
	switch (Value)
	{
	case APIInactiveOrderStatus::NameEnum::Inactive:
		return TEXT("INACTIVE");
	}

	UE_LOG(LogImmutablezkEVMAPI, Error, TEXT("Invalid APIInactiveOrderStatus::NameEnum Value (%d)"), (int)Value);
	return TEXT("");
}

FString APIInactiveOrderStatus::EnumToString(const APIInactiveOrderStatus::NameEnum& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, APIInactiveOrderStatus::NameEnum& Value)
{
	static TMap<FString, APIInactiveOrderStatus::NameEnum> StringToEnum = { 
		{ TEXT("INACTIVE"), APIInactiveOrderStatus::NameEnum::Inactive }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool APIInactiveOrderStatus::EnumFromString(const FString& EnumAsString, APIInactiveOrderStatus::NameEnum& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const APIInactiveOrderStatus::NameEnum& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, APIInactiveOrderStatus::NameEnum& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void APIInactiveOrderStatus::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("name")); WriteJsonValue(Writer, Name);
	Writer->WriteIdentifierPrefix(TEXT("sufficient_approvals")); WriteJsonValue(Writer, SufficientApprovals);
	Writer->WriteIdentifierPrefix(TEXT("sufficient_balances")); WriteJsonValue(Writer, SufficientBalances);
	Writer->WriteObjectEnd();
}

bool APIInactiveOrderStatus::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("name"), Name);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("sufficient_approvals"), SufficientApprovals);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("sufficient_balances"), SufficientBalances);

	return ParseSuccess;
}

}
