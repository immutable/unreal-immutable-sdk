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
 * APINativeItem
 *
 * 
 */
class IMMUTABLEORDERBOOK_API APINativeItem : public Model
{
public:
    virtual ~APINativeItem() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	FString Amount;
	enum class TypeEnum
	{
		Native,
  	};

	static FString EnumToString(const TypeEnum& EnumValue);
	static bool EnumFromString(const FString& EnumAsString, TypeEnum& EnumValue);
	TypeEnum Type;
};

}
