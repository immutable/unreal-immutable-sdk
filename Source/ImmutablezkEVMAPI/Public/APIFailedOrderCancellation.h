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
 * APIFailedOrderCancellation
 *
 * 
 */
class IMMUTABLEZKEVMAPI_API APIFailedOrderCancellation : public Model
{
public:
    virtual ~APIFailedOrderCancellation() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	/* ID of the order which failed to be cancelled */
	FString Order;
	enum class ReasonCodeEnum
	{
		Filled,
  	};

	static FString EnumToString(const ReasonCodeEnum& EnumValue);
	static bool EnumFromString(const FString& EnumAsString, ReasonCodeEnum& EnumValue);
	/* Reason code indicating why the order failed to be cancelled */
	ReasonCodeEnum ReasonCode;
};

}
