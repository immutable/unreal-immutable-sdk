/**
 * TS SDK API
 * running ts sdk as an api
 *
 * OpenAPI spec version: 1.0.0
 * Contact: contact@immutable.com
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * https://github.com/OpenAPITools/openapi-generator
 * Do not edit the class manually.
 */

#pragma once

#include "APIBaseModel.h"

namespace ImmutableOrderbook
{

/*
 * APIOrderStatusOneOf4
 *
 * 
 */
class IMMUTABLEORDERBOOK_API APIOrderStatusOneOf4 : public Model
{
public:
    virtual ~APIOrderStatusOneOf4() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	enum class NameEnum
	{
		Inactive,
  	};

	static FString EnumToString(const NameEnum& EnumValue);
	static bool EnumFromString(const FString& EnumAsString, NameEnum& EnumValue);
	/* The order status that indicates an order cannot be fulfilled. */
	TOptional<NameEnum> Name;
	/* Whether the order offerer has sufficient approvals */
	TOptional<bool> SufficientApprovals;
	/* Whether the order offerer still has sufficient balance to complete the order */
	TOptional<bool> SufficientBalances;
};

}
