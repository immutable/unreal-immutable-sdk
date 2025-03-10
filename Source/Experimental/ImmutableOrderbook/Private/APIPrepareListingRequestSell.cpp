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

#include "APIPrepareListingRequestSell.h"

#include "ImmutableOrderbookModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutableOrderbook
{

inline FString ToString(const APIPrepareListingRequestSell::TypeEnum& Value)
{
	switch (Value)
	{
	case APIPrepareListingRequestSell::TypeEnum::ERC1155:
		return TEXT("ERC1155");
	case APIPrepareListingRequestSell::TypeEnum::ERC721:
		return TEXT("ERC721");
	}

	UE_LOG(LogImmutableOrderbook, Error, TEXT("Invalid APIPrepareListingRequestSell::TypeEnum Value (%d)"), (int)Value);
	return TEXT("");
}

FString APIPrepareListingRequestSell::EnumToString(const APIPrepareListingRequestSell::TypeEnum& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, APIPrepareListingRequestSell::TypeEnum& Value)
{
	static TMap<FString, APIPrepareListingRequestSell::TypeEnum> StringToEnum = { 
		{ TEXT("ERC1155"), APIPrepareListingRequestSell::TypeEnum::ERC1155 },
		{ TEXT("ERC721"), APIPrepareListingRequestSell::TypeEnum::ERC721 }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool APIPrepareListingRequestSell::EnumFromString(const FString& EnumAsString, APIPrepareListingRequestSell::TypeEnum& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const APIPrepareListingRequestSell::TypeEnum& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, APIPrepareListingRequestSell::TypeEnum& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void APIPrepareListingRequestSell::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	// -- DIVERGE
	if (Amount.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("amount")); WriteJsonValue(Writer, Amount.GetValue());
	}
	// -- DIVERGE
	Writer->WriteIdentifierPrefix(TEXT("contractAddress")); WriteJsonValue(Writer, ContractAddress);
	Writer->WriteIdentifierPrefix(TEXT("tokenId")); WriteJsonValue(Writer, TokenId);
	Writer->WriteIdentifierPrefix(TEXT("type")); WriteJsonValue(Writer, Type);
	Writer->WriteObjectEnd();
}

bool APIPrepareListingRequestSell::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("amount"), Amount);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("contractAddress"), ContractAddress);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("tokenId"), TokenId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("type"), Type);

	return ParseSuccess;
}

}
