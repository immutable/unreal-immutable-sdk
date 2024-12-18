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

#include "APIOrderComponents.h"

#include "ImmutableOrderbookModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutableOrderbook
{

void APIOrderComponents::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("conduitKey")); WriteJsonValue(Writer, ConduitKey);
	Writer->WriteIdentifierPrefix(TEXT("consideration")); WriteJsonValue(Writer, Consideration);
	Writer->WriteIdentifierPrefix(TEXT("endTime")); WriteJsonValue(Writer, EndTime);
	Writer->WriteIdentifierPrefix(TEXT("offer")); WriteJsonValue(Writer, Offer);
	Writer->WriteIdentifierPrefix(TEXT("offerer")); WriteJsonValue(Writer, Offerer);
	Writer->WriteIdentifierPrefix(TEXT("orderType")); WriteJsonValue(Writer, OrderType);
	Writer->WriteIdentifierPrefix(TEXT("salt")); WriteJsonValue(Writer, Salt);
	Writer->WriteIdentifierPrefix(TEXT("startTime")); WriteJsonValue(Writer, StartTime);
	Writer->WriteIdentifierPrefix(TEXT("totalOriginalConsiderationItems")); WriteJsonValue(Writer, TotalOriginalConsiderationItems);
	Writer->WriteIdentifierPrefix(TEXT("zone")); WriteJsonValue(Writer, Zone);
	Writer->WriteIdentifierPrefix(TEXT("zoneHash")); WriteJsonValue(Writer, ZoneHash);
	Writer->WriteIdentifierPrefix(TEXT("counter")); WriteJsonValue(Writer, Counter);
	Writer->WriteObjectEnd();
}

bool APIOrderComponents::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("conduitKey"), ConduitKey);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("consideration"), Consideration);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("endTime"), EndTime);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("offer"), Offer);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("offerer"), Offerer);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("orderType"), OrderType);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("salt"), Salt);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("startTime"), StartTime);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("totalOriginalConsiderationItems"), TotalOriginalConsiderationItems);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("zone"), Zone);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("zoneHash"), ZoneHash);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("counter"), Counter);

	return ParseSuccess;
}

}
