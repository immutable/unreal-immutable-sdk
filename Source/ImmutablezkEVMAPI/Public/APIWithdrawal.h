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
 * APIWithdrawal
 *
 * The withdrawal activity details
 */
class IMMUTABLEZKEVMAPI_API APIWithdrawal : public Model
{
public:
    virtual ~APIWithdrawal() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	/* The account address the asset was withdrawn from */
	FString From;
	/* The amount of assets withdrawn */
	FString Amount;
	APIActivityAsset Asset;
};

}