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
 * APIOperatorAllowlistStatusUpdated
 *
 * The update details
 */
class IMMUTABLEZKEVMAPI_API APIOperatorAllowlistStatusUpdated : public Model
{
public:
    virtual ~APIOperatorAllowlistStatusUpdated() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	/* Why this action was performed */
	FString Reason;
};

}
