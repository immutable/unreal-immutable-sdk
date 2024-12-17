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

#include "APIPrepareOrderCancellationsRequest.h"

#include "ImmutableOrderbookModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutableOrderbook
{

void APIPrepareOrderCancellationsRequest::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("orderIds")); WriteJsonValue(Writer, OrderIds);
	Writer->WriteObjectEnd();
}

bool APIPrepareOrderCancellationsRequest::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("orderIds"), OrderIds);

	return ParseSuccess;
}

}