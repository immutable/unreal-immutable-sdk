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
#include "APIChain.h"
#include "APIFee.h"
#include "APIFillStatus.h"
#include "APIItem.h"
#include "APIOrderStatus.h"
#include "APIProtocolData.h"

namespace ImmutablezkEVMAPI
{

/*
 * APIOrder
 *
 * 
 */
class IMMUTABLEZKEVMAPI_API APIOrder : public Model
{
public:
    virtual ~APIOrder() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	FString AccountAddress;
	TArray<APIItem> Buy;
	TArray<APIFee> Fees;
	APIChain Chain;
	/* Time the Order is created */
	FDateTime CreatedAt;
	/* Time after which the Order is considered expired */
	FDateTime EndAt;
	/* Global Order identifier */
	FString Id;
	FString OrderHash;
	APIProtocolData ProtocolData;
	/* A random value added to the create Order request */
	FString Salt;
	TArray<APIItem> Sell;
	/* Digital signature generated by the user for the specific Order */
	FString Signature;
	/* Time after which Order is considered active */
	FDateTime StartAt;
	APIOrderStatus Status;
	enum class TypeEnum
	{
		Listing,
		Bid,
		CollectionBid,
  	};

	static FString EnumToString(const TypeEnum& EnumValue);
	static bool EnumFromString(const FString& EnumAsString, TypeEnum& EnumValue);
	/* Order type */
	TypeEnum Type;
	/* Time the Order is last updated */
	FDateTime UpdatedAt;
	APIFillStatus FillStatus;
};

}