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
 * APIOrderStatusName
 *
 * The Order status
 */
class IMMUTABLEZKEVMAPI_API APIOrderStatusName : public Model
{
public:
    virtual ~APIOrderStatusName() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	enum class Values
	{
		Pending,
		Active,
		Inactive,
		Filled,
		Expired,
		Cancelled,
  	};

	Values Value;

	static FString EnumToString(const Values& EnumValue);
	static bool EnumFromString(const FString& EnumAsString, Values& EnumValue);
};

}
