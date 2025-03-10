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

#include "APINFTWithBalance.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

void APINFTWithBalance::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("chain")); WriteJsonValue(Writer, Chain);
	Writer->WriteIdentifierPrefix(TEXT("token_id")); WriteJsonValue(Writer, TokenId);
	Writer->WriteIdentifierPrefix(TEXT("contract_address")); WriteJsonValue(Writer, ContractAddress);
	Writer->WriteIdentifierPrefix(TEXT("contract_type")); WriteJsonValue(Writer, ContractType);
	Writer->WriteIdentifierPrefix(TEXT("indexed_at")); WriteJsonValue(Writer, IndexedAt);
	Writer->WriteIdentifierPrefix(TEXT("updated_at")); WriteJsonValue(Writer, UpdatedAt);
	if (MetadataSyncedAt.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("metadata_synced_at")); WriteJsonValue(Writer, MetadataSyncedAt.GetValue());
	}
	if (MetadataId.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("metadata_id")); WriteJsonValue(Writer, MetadataId.GetValue());
	}
	if (Name.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("name")); WriteJsonValue(Writer, Name.GetValue());
	}
	if (Description.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("description")); WriteJsonValue(Writer, Description.GetValue());
	}
	if (Image.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("image")); WriteJsonValue(Writer, Image.GetValue());
	}
	if (ExternalLink.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("external_link")); WriteJsonValue(Writer, ExternalLink.GetValue());
	}
	if (AnimationUrl.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("animation_url")); WriteJsonValue(Writer, AnimationUrl.GetValue());
	}
	if (YoutubeUrl.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("youtube_url")); WriteJsonValue(Writer, YoutubeUrl.GetValue());
	}
	Writer->WriteIdentifierPrefix(TEXT("attributes")); WriteJsonValue(Writer, Attributes);
	Writer->WriteIdentifierPrefix(TEXT("balance")); WriteJsonValue(Writer, Balance);
	Writer->WriteObjectEnd();
}

bool APINFTWithBalance::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("chain"), Chain);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("token_id"), TokenId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("contract_address"), ContractAddress);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("contract_type"), ContractType);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("indexed_at"), IndexedAt);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("updated_at"), UpdatedAt);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("metadata_synced_at"), MetadataSyncedAt);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("metadata_id"), MetadataId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("name"), Name);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("description"), Description);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("image"), Image);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("external_link"), ExternalLink);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("animation_url"), AnimationUrl);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("youtube_url"), YoutubeUrl);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("attributes"), Attributes);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("balance"), Balance);

	return ParseSuccess;
}

}
