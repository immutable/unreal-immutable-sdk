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

#include "APIMintAsset.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

void APIMintAsset::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("reference_id")); WriteJsonValue(Writer, ReferenceId);
	Writer->WriteIdentifierPrefix(TEXT("owner_address")); WriteJsonValue(Writer, OwnerAddress);
	if (TokenId.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("token_id")); WriteJsonValue(Writer, TokenId.GetValue());
	}
	if (Amount.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("amount")); WriteJsonValue(Writer, Amount.GetValue());
	}
	if (Metadata.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("metadata")); WriteJsonValue(Writer, Metadata.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool APIMintAsset::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("reference_id"), ReferenceId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("owner_address"), OwnerAddress);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("token_id"), TokenId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("amount"), Amount);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("metadata"), Metadata);

	return ParseSuccess;
}

}
