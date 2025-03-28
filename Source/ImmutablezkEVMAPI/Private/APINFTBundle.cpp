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

#include "APINFTBundle.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

void APINFTBundle::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("nft_with_stack")); WriteJsonValue(Writer, NftWithStack);
	if (Market.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("market")); WriteJsonValue(Writer, Market.GetValue());
	}
	Writer->WriteIdentifierPrefix(TEXT("listings")); WriteJsonValue(Writer, Listings);
	Writer->WriteIdentifierPrefix(TEXT("bids")); WriteJsonValue(Writer, Bids);
	Writer->WriteObjectEnd();
}

bool APINFTBundle::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("nft_with_stack"), NftWithStack);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("market"), Market);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("listings"), Listings);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("bids"), Bids);

	return ParseSuccess;
}

}
