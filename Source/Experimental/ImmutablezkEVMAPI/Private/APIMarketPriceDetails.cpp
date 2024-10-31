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

#include "APIMarketPriceDetails.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

void APIMarketPriceDetails::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("token")); WriteJsonValue(Writer, Token);
	Writer->WriteIdentifierPrefix(TEXT("amount")); WriteJsonValue(Writer, Amount);
	Writer->WriteIdentifierPrefix(TEXT("fee_inclusive_amount")); WriteJsonValue(Writer, FeeInclusiveAmount);
	Writer->WriteIdentifierPrefix(TEXT("fees")); WriteJsonValue(Writer, Fees);
	if (ConvertedPrices.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("converted_prices")); WriteJsonValue(Writer, ConvertedPrices.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool APIMarketPriceDetails::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("token"), Token);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("amount"), Amount);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("fee_inclusive_amount"), FeeInclusiveAmount);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("fees"), Fees);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("converted_prices"), ConvertedPrices);

	return ParseSuccess;
}

}
