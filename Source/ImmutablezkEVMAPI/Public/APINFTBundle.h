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
#include "APIListing.h"
#include "APIMarket.h"
#include "APINFTWithStack.h"

namespace ImmutablezkEVMAPI
{

/*
 * APINFTBundle
 *
 * NFT bundle includes NFT with stack, markets and listings
 */
class IMMUTABLEZKEVMAPI_API APINFTBundle : public Model
{
public:
    virtual ~APINFTBundle() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	APINFTWithStack NftWithStack;
	TOptional<APIMarket> Market;
	/* List of open listings for the stack. */
	TArray<APIListing> Listings;
};

}