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
 * APIOperatorAllowlistStatusRequested
 *
 * The request details
 */
class IMMUTABLEZKEVMAPI_API APIOperatorAllowlistStatusRequested : public Model
{
public:
    virtual ~APIOperatorAllowlistStatusRequested() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	/* Reason this contract needs to be added */
	FString Purpose;
	/* Attestation of whether this contract is a settlement contract */
	bool IsSettlementContract = false;
};

}