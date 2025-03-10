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
#include "APIItemType.h"

namespace ImmutableOrderbook
{

/*
 * APIOrderComponentsConsiderationInner
 *
 * 
 */
class IMMUTABLEORDERBOOK_API APIOrderComponentsConsiderationInner : public Model
{
public:
    virtual ~APIOrderComponentsConsiderationInner() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	FString EndAmount;
	FString IdentifierOrCriteria;
	APIItemType ItemType;
	FString Recipient;
	FString StartAmount;
	FString Token;
};

}
