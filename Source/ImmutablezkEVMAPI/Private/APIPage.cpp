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

#include "APIPage.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

void APIPage::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	if (PreviousCursor.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("previous_cursor")); WriteJsonValue(Writer, PreviousCursor.GetValue());
	}
	if (NextCursor.IsSet())
	{
		Writer->WriteIdentifierPrefix(TEXT("next_cursor")); WriteJsonValue(Writer, NextCursor.GetValue());
	}
	Writer->WriteObjectEnd();
}

bool APIPage::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("previous_cursor"), PreviousCursor);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("next_cursor"), NextCursor);

	return ParseSuccess;
}

}
