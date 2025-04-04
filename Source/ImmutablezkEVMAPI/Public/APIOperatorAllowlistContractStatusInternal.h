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
#include "APIOperatorAllowlistStatus.h"
#include "APIOperatorAllowlistStatusDetails.h"
#include "APIOrganisationTier.h"

namespace ImmutablezkEVMAPI
{

/*
 * APIOperatorAllowlistContractStatusInternal
 *
 * 
 */
class IMMUTABLEZKEVMAPI_API APIOperatorAllowlistContractStatusInternal : public Model
{
public:
    virtual ~APIOperatorAllowlistContractStatusInternal() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	APIChain Chain;
	/* The address of the contract */
	FString ContractAddress;
	APIOperatorAllowlistStatus Status;
	APIOperatorAllowlistStatusDetails Details;
	/* When the contract status was created */
	FDateTime CreatedAt;
	/* Who created the status */
	FString CreatedBy;
	/* The name of the organisation associated with this contract */
	TOptional<FString> OrgName;
	TOptional<APIOrganisationTier> OrgTier;
};

}
