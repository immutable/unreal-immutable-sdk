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
 * APIOrderProtocolData
 *
 * 
 */
class IMMUTABLEORDERBOOK_API APIOrderProtocolData : public Model
{
public:
    virtual ~APIOrderProtocolData() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	TOptional<FString> Counter;
	enum class OrderTypeEnum
	{
		FullRestricted,
		PartialRestricted,
  	};

	static FString EnumToString(const OrderTypeEnum& EnumValue);
	static bool EnumFromString(const FString& EnumAsString, OrderTypeEnum& EnumValue);
	TOptional<OrderTypeEnum> OrderType;
	TOptional<FString> SeaportAddress;
	TOptional<FString> SeaportVersion;
	TOptional<FString> ZoneAddress;
};

}