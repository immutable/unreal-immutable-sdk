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

#include "APISignCraftingRequest.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

void APISignCraftingRequest::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("multi_caller")); WriteJsonValue(Writer, MultiCaller);
	Writer->WriteIdentifierPrefix(TEXT("reference_id")); WriteJsonValue(Writer, ReferenceId);
	Writer->WriteIdentifierPrefix(TEXT("calls")); WriteJsonValue(Writer, Calls);
	Writer->WriteIdentifierPrefix(TEXT("expires_at")); WriteJsonValue(Writer, ExpiresAt);
	Writer->WriteObjectEnd();
}

bool APISignCraftingRequest::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("multi_caller"), MultiCaller);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("reference_id"), ReferenceId);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("calls"), Calls);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("expires_at"), ExpiresAt);

	return ParseSuccess;
}

}