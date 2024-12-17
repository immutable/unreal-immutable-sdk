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

#include "APIMintRequestErrorMessage.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

void APIMintRequestErrorMessage::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	if (Message.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("message")); WriteJsonValue(Writer, Message.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool APIMintRequestErrorMessage::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("message"), Message);

	return ParseSuccess;
}

}