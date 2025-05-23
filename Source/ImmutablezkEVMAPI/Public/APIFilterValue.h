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

namespace ImmutablezkEVMAPI
{

/*
 * APIFilterValue
 *
 * 
 */
class IMMUTABLEZKEVMAPI_API APIFilterValue : public Model
{
public:
    virtual ~APIFilterValue() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	FString Value;
	/* Number of NFTs that have this trait. Uint256 as string */
	FString NftCount;
};

}
