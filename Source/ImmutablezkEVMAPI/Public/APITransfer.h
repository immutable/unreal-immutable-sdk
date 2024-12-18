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
 * APITransfer
 *
 * The transfer activity details
 */
class IMMUTABLEZKEVMAPI_API APITransfer : public Model
{
public:
    virtual ~APITransfer() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	/* The account address the asset was transferred from */
	FString From;
	/* The account address the asset was transferred to */
	FString To;
	/* The amount of assets transferred */
	FString Amount;
	APIActivityAsset Asset;
};

}
