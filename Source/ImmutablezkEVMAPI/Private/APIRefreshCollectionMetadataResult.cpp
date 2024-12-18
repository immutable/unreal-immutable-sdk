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

#include "APIRefreshCollectionMetadataResult.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Templates/SharedPointer.h"

namespace ImmutablezkEVMAPI
{

void APIRefreshCollectionMetadataResult::WriteJson(JsonWriter& Writer) const
{
	Writer->WriteObjectStart();
	Writer->WriteIdentifierPrefix(TEXT("contract_address")); WriteJsonValue(Writer, ContractAddress);
	Writer->WriteIdentifierPrefix(TEXT("chain")); WriteJsonValue(Writer, Chain);
	Writer->WriteIdentifierPrefix(TEXT("collection_metadata")); WriteJsonValue(Writer, CollectionMetadata);
	Writer->WriteObjectEnd();
}

bool APIRefreshCollectionMetadataResult::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	const TSharedPtr<FJsonObject>* Object;
	if (!JsonValue->TryGetObject(Object))
		return false;

	bool ParseSuccess = true;

	ParseSuccess &= TryGetJsonValue(*Object, TEXT("contract_address"), ContractAddress);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("chain"), Chain);
	ParseSuccess &= TryGetJsonValue(*Object, TEXT("collection_metadata"), CollectionMetadata);

	return ParseSuccess;
}

}
