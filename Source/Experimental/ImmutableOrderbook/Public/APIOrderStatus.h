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
#include "APICancellationType.h"
#include "APIOrderStatusOneOf.h"
#include "APIOrderStatusOneOf1.h"
#include "APIOrderStatusOneOf2.h"
#include "APIOrderStatusOneOf3.h"
#include "APIOrderStatusOneOf4.h"
#include "APIOrderStatusOneOf5.h"

namespace ImmutableOrderbook
{

/*
 * APIOrderStatus
 *
 * The Order status
 */
class IMMUTABLEORDERBOOK_API APIOrderStatus : public Model
{
public:
    virtual ~APIOrderStatus() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	enum class NameEnum
	{
		Pending,
  	};

	static FString EnumToString(const NameEnum& EnumValue);
	static bool EnumFromString(const FString& EnumAsString, NameEnum& EnumValue);
	/* The order status that indicates the order is yet to be active due to various reasons. */
	TOptional<NameEnum> Name;
	TOptional<APICancellationType> CancellationType;
	/* Whether the cancellation of the order is pending */
	TOptional<bool> Pending;
	/* Whether the order offerer has sufficient approvals */
	TOptional<bool> SufficientApprovals;
	/* Whether the order offerer still has sufficient balance to complete the order */
	TOptional<bool> SufficientBalances;
	/* Whether the order has been evaluated after its creation */
	TOptional<bool> Evaluated;
	/* Whether the order has reached its specified start time */
	TOptional<bool> Started;
};

}