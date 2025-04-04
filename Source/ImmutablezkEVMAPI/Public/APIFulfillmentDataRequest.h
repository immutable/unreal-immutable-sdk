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

#pragma once

#include "APIBaseModel.h"
#include "APIFee.h"

namespace ImmutablezkEVMAPI
{

/*
 * APIFulfillmentDataRequest
 *
 * 
 */
class IMMUTABLEZKEVMAPI_API APIFulfillmentDataRequest : public Model
{
public:
    virtual ~APIFulfillmentDataRequest() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	FGuid OrderId;
	/* Address of the intended account fulfilling the order */
	FString TakerAddress;
	TArray<APIFee> Fees;
	/* Token ID for the ERC721 or ERC1155 token when fulfilling a collection order */
	TOptional<FString> TokenId;
};

}
