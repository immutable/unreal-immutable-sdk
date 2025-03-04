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
#include "APIActivityAsset.h"

namespace ImmutablezkEVMAPI
{

/*
 * APIBurn
 *
 * The burn activity details
 */
class IMMUTABLEZKEVMAPI_API APIBurn : public Model
{
public:
    virtual ~APIBurn() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	/* The account address the asset was transferred from */
	FString From;
	/* The amount of assets burnt */
	FString Amount;
	APIActivityAsset Asset;
};

}
