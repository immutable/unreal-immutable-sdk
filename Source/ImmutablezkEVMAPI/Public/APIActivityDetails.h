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

#include "Misc/TVariant.h"

#include "APIBaseModel.h"
#include "APIActivityAsset.h"
#include "APIBurn.h"
#include "APIDeposit.h"
#include "APIMint.h"
#include "APINFTSale.h"
#include "APISalePayment.h"
#include "APITransfer.h"
#include "APIWithdrawal.h"

namespace ImmutablezkEVMAPI
{

/*
 * APIActivityDetails
 *
 * The activity details
 */
class IMMUTABLEZKEVMAPI_API APIActivityDetails : public Model
{
public:
    virtual ~APIActivityDetails() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	TVariant<APIBurn, APIDeposit, APIMint, APINFTSale, APITransfer, APIWithdrawal> OneOf;
};

}
