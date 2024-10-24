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

#include "APINFTWithStack.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

void APINFTWithStack::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("token_id")); WriteJsonValue(Writer, TokenId);
	Writer->WriteIdentifierPrefix(TEXT("stack_id")); WriteJsonValue(Writer, StackId);
	Writer->WriteIdentifierPrefix(TEXT("chain")); WriteJsonValue(Writer, Chain);
	Writer->WriteIdentifierPrefix(TEXT("contract_address")); WriteJsonValue(Writer, ContractAddress);
	Writer->WriteIdentifierPrefix(TEXT("contract_type")); WriteJsonValue(Writer, ContractType);
	Writer->WriteIdentifierPrefix(TEXT("created_at")); WriteJsonValue(Writer, CreatedAt);
	Writer->WriteIdentifierPrefix(TEXT("updated_at")); WriteJsonValue(Writer, UpdatedAt);
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
	if (ExternalUrl.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("external_url")); WriteJsonValue(Writer, ExternalUrl.GetValue());
	}
	if (AnimationUrl.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("animation_url")); WriteJsonValue(Writer, AnimationUrl.GetValue());
	}
	if (YoutubeUrl.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("youtube_url")); WriteJsonValue(Writer, YoutubeUrl.GetValue());
	}
	if (Attributes.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("attributes")); WriteJsonValue(Writer, Attributes.GetValue());
	}
	if (Balance.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("balance")); WriteJsonValue(Writer, Balance.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool APINFTWithStack::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("token_id"), TokenId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("stack_id"), StackId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("chain"), Chain);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("contract_address"), ContractAddress);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("contract_type"), ContractType);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("created_at"), CreatedAt);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("updated_at"), UpdatedAt);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("name"), Name);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("description"), Description);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("image"), Image);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("external_url"), ExternalUrl);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("animation_url"), AnimationUrl);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("youtube_url"), YoutubeUrl);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("attributes"), Attributes);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("balance"), Balance);

	return ParseSuccess;
}

}
