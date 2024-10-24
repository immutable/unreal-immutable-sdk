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

#include "APIOrderComponentsOfferInner.h"

#include "ImmutableOrderbookModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutableOrderbook
{

void APIOrderComponentsOfferInner::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("endAmount")); WriteJsonValue(Writer, EndAmount);
	Writer->WriteIdentifierPrefix(TEXT("identifierOrCriteria")); WriteJsonValue(Writer, IdentifierOrCriteria);
	Writer->WriteIdentifierPrefix(TEXT("itemType")); WriteJsonValue(Writer, ItemType);
	Writer->WriteIdentifierPrefix(TEXT("startAmount")); WriteJsonValue(Writer, StartAmount);
	Writer->WriteIdentifierPrefix(TEXT("token")); WriteJsonValue(Writer, Token);
	Writer->WriteObjectEnd();
}

bool APIOrderComponentsOfferInner::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("endAmount"), EndAmount);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("identifierOrCriteria"), IdentifierOrCriteria);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("itemType"), ItemType);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("startAmount"), StartAmount);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("token"), Token);

	return ParseSuccess;
}

}
