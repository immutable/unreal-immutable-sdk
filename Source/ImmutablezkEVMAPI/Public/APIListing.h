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
#include "APIMarketPriceDetails.h"

namespace ImmutablezkEVMAPI
{

/*
 * APIListing
 *
 * 
 */
class IMMUTABLEZKEVMAPI_API APIListing : public Model
{
public:
    virtual ~APIListing() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	/* Global Order identifier */
	FString ListingId;
	APIMarketPriceDetails PriceDetails;
	/* Token ID */
	FString TokenId;
	/* ETH Address of collection that the asset belongs to */
	FString ContractAddress;
	/* ETH Address of listing creator */
	FString Creator;
	/* Amount of token included in the listing */
	FString Amount;
};

}
