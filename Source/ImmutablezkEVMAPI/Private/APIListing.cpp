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

#include "APIListing.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

void APIListing::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("listing_id")); WriteJsonValue(Writer, ListingId);
	Writer->WriteIdentifierPrefix(TEXT("price_details")); WriteJsonValue(Writer, PriceDetails);
	Writer->WriteIdentifierPrefix(TEXT("token_id")); WriteJsonValue(Writer, TokenId);
	Writer->WriteIdentifierPrefix(TEXT("contract_address")); WriteJsonValue(Writer, ContractAddress);
	Writer->WriteIdentifierPrefix(TEXT("creator")); WriteJsonValue(Writer, Creator);
	Writer->WriteIdentifierPrefix(TEXT("amount")); WriteJsonValue(Writer, Amount);
	Writer->WriteObjectEnd();
}

bool APIListing::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("listing_id"), ListingId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("price_details"), PriceDetails);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("token_id"), TokenId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("contract_address"), ContractAddress);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("creator"), Creator);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("amount"), Amount);

	return ParseSuccess;
}

}
