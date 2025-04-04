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

#include "APINFTSale.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

void APINFTSale::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("order_id")); WriteJsonValue(Writer, OrderId);
	Writer->WriteIdentifierPrefix(TEXT("to")); WriteJsonValue(Writer, To);
	Writer->WriteIdentifierPrefix(TEXT("from")); WriteJsonValue(Writer, From);
	Writer->WriteIdentifierPrefix(TEXT("asset")); WriteJsonValue(Writer, Asset);
	Writer->WriteIdentifierPrefix(TEXT("payment")); WriteJsonValue(Writer, Payment);
	Writer->WriteObjectEnd();
}

bool APINFTSale::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("order_id"), OrderId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("to"), To);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("from"), From);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("asset"), Asset);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("payment"), Payment);

	return ParseSuccess;
}

}
