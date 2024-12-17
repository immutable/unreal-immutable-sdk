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

#include "APIAssetCollectionItem.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

inline FString ToString(const APIAssetCollectionItem::TypeEnum& Value)
{
	switch (Value)
	{
	case APIAssetCollectionItem::TypeEnum::ERC721COLLECTION:
		return TEXT("ERC721_COLLECTION");
	case APIAssetCollectionItem::TypeEnum::ERC1155COLLECTION:
		return TEXT("ERC1155_COLLECTION");
	}

	UE_LOG(LogImmutablezkEVMAPI, Error, TEXT("Invalid APIAssetCollectionItem::TypeEnum Value (%d)"), (int)Value);
	return TEXT("");
}

FString APIAssetCollectionItem::EnumToString(const APIAssetCollectionItem::TypeEnum& EnumValue)
{
	return ToString(EnumValue);
}

inline bool FromString(const FString& EnumAsString, APIAssetCollectionItem::TypeEnum& Value)
{
	static TMap<FString, APIAssetCollectionItem::TypeEnum> StringToEnum = { 
		{ TEXT("ERC721_COLLECTION"), APIAssetCollectionItem::TypeEnum::ERC721COLLECTION },
		{ TEXT("ERC1155_COLLECTION"), APIAssetCollectionItem::TypeEnum::ERC1155COLLECTION }, };

	const auto Found = StringToEnum.Find(EnumAsString);
	if(Found)
		Value = *Found;

	return Found != nullptr;
}

bool APIAssetCollectionItem::EnumFromString(const FString& EnumAsString, APIAssetCollectionItem::TypeEnum& EnumValue)
{
	return FromString(EnumAsString, EnumValue);
}

inline void WriteJsonValue(JsonWriter& Writer, const APIAssetCollectionItem::TypeEnum& Value)
{
	WriteJsonValue(Writer, ToString(Value));
}

inline bool TryGetJsonValue(const TSharedPtr<FJsonValue>& JsonValue, APIAssetCollectionItem::TypeEnum& Value)
{
	FString TmpValue;
	if (JsonValue->TryGetString(TmpValue))
	{
		if(FromString(TmpValue, Value))
			return true;
	}
	return false;
}

void APIAssetCollectionItem::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("type")); WriteJsonValue(Writer, Type);
	Writer->WriteIdentifierPrefix(TEXT("contract_address")); WriteJsonValue(Writer, ContractAddress);
	Writer->WriteIdentifierPrefix(TEXT("amount")); WriteJsonValue(Writer, Amount);
	Writer->WriteObjectEnd();
}

bool APIAssetCollectionItem::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("type"), Type);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("contract_address"), ContractAddress);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("amount"), Amount);

	return ParseSuccess;
}

}