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
#include "APINFTWithBalance.h"
#include "APIPage.h"

namespace ImmutablezkEVMAPI
{

/*
 * APIListNFTsByOwnerResult
 *
 * 
 */
class IMMUTABLEZKEVMAPI_API APIListNFTsByOwnerResult : public Model
{
public:
    virtual ~APIListNFTsByOwnerResult() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	/* List of NFTs by owner */
	TArray<APINFTWithBalance> Result;
	APIPage Page;
};

}
