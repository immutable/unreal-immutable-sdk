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
#include "APIFilterValue.h"

namespace ImmutablezkEVMAPI
{

/*
 * APIFilter
 *
 * 
 */
class IMMUTABLEZKEVMAPI_API APIFilter : public Model
{
public:
    virtual ~APIFilter() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	/* Name of trait */
	FString Name;
	/* List of 100 most common values for this trait sorted by number of associated NFTs */
	TArray<APIFilterValue> Values;
	/* Indicated how many more distinct values exist */
	int32 OmittedValuesCount = 0;
};

}
