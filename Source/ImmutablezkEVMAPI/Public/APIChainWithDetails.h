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
 * APIChainWithDetails
 *
 * 
 */
class IMMUTABLEZKEVMAPI_API APIChainWithDetails : public Model
{
public:
    virtual ~APIChainWithDetails() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	/* The id of chain */
	FString Id;
	/* The name of chain */
	FString Name;
	/* URL for RPC node */
	TOptional<FString> RpcUrl;
	/* The address of the Operator Allowlist - https://docs.immutable.com/products/zkevm/minting/royalties/allowlist-spec/ */
	TOptional<FString> OperatorAllowlistAddress;
	/* The address of the minter used in the Minting API - https://docs.immutable.com/products/zkEVM/minting/minting-api#minting-api-prerequisites */
	TOptional<FString> MinterAddress;
};

}