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
#include "APIItem.h"
#include "APITradeBlockchainMetadata.h"

namespace ImmutablezkEVMAPI
{

/*
 * APITrade
 *
 * 
 */
class IMMUTABLEZKEVMAPI_API APITrade : public Model
{
public:
    virtual ~APITrade() {}
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;
	void WriteJson(JsonWriter& Writer) const final;

	/* Buy items are transferred from the taker to the maker. */
	TArray<APIItem> Buy;
	/* Deprecated. Use maker and taker addresses instead of buyer and seller addresses. */
	FString BuyerAddress;
	/* Deprecated. Use fees instead. The taker always pays the fees. */
	TArray<APIFee> BuyerFees;
	TArray<APIFee> Fees;
	APIChain Chain;
	FString OrderId;
	TOptional<APITradeBlockchainMetadata> BlockchainMetadata;
	/* Time the on-chain trade event is indexed by the order book system */
	FDateTime IndexedAt;
	/* Global Trade identifier */
	FString Id;
	/* Sell items are transferred from the maker to the taker. */
	TArray<APIItem> Sell;
	/* Deprecated. Use maker and taker addresses instead of buyer and seller addresses. */
	FString SellerAddress;
	FString MakerAddress;
	FString TakerAddress;
};

}
