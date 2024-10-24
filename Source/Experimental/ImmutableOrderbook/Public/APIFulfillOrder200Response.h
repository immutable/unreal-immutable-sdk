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
#include "APIOrder.h"
#include "APITransactionAction.h"

namespace ImmutableOrderbook
{

/*
 * APIFulfillOrder200Response
 *
 * Response schema for the fulfillOrder endpoint
 */
class IMMUTABLEORDERBOOK_API APIFulfillOrder200Response : public Model
{
public:
    virtual ~APIFulfillOrder200Response() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	TOptional<TArray<APITransactionAction>> Actions;
	/* User MUST submit the fulfillment transaction before the expiration Submitting after the expiration will result in a on chain revert */
	TOptional<FString> Expiration;
	TOptional<APIOrder> Order;
};

}
