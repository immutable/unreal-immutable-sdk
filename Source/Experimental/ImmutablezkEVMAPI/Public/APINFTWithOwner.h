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

namespace ImmutablezkEVMAPI
{

/*
 * APINFTWithOwner
 *
 * 
 */
class IMMUTABLEZKEVMAPI_API APINFTWithOwner : public Model
{
public:
    virtual ~APINFTWithOwner() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	APIChain Chain;
	/* The address of NFT contract */
	FString ContractAddress;
	/* An `uint256` token id as string */
	FString TokenId;
	/* The account address of the owner of the NFT */
	FString AccountAddress;
	/* The amount of owned tokens (uint256 as string) */
	FString Balance;
	/* When the owner last changed for the given NFT */
	FDateTime UpdatedAt;
};

}
