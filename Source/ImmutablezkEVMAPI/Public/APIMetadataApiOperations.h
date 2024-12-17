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
#include "APIMetadataApi.h"

#include "APIAPIError400.h"
#include "APIAPIError401.h"
#include "APIAPIError403.h"
#include "APIAPIError404.h"
#include "APIAPIError429.h"
#include "APIAPIError500.h"
#include "APIGetMetadataResult.h"
#include "APIListMetadataResult.h"
#include "APIMetadataRefreshRateLimitResult.h"
#include "APIRefreshMetadataByIDRequest.h"
#include "APIRefreshNFTMetadataByTokenIDRequest.h"
#include "APIStackBundle.h"

namespace ImmutablezkEVMAPI
{

/* Get metadata by ID
 *
 * Get metadata by ID
*/
class IMMUTABLEZKEVMAPI_API APIMetadataApi::GetMetadataRequest : public Request
{
public:
    virtual ~GetMetadataRequest() {}
	void SetupHttpRequest(const FHttpRequestRef& HttpRequest) const final;
	FString ComputePath() const final;

	/* The name of chain */
	FString ChainName;
	/* The address of metadata contract */
	FString ContractAddress;
	/* The id of the metadata */
	FGuid MetadataId;
};

class IMMUTABLEZKEVMAPI_API APIMetadataApi::GetMetadataResponse : public Response
{
public:
    virtual ~GetMetadataResponse() {}
	void SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode) final;
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;

    APIGetMetadataResult Content;
};

/* Get a list of metadata from the given contract
 *
 * Get a list of metadata from the given contract
*/
class IMMUTABLEZKEVMAPI_API APIMetadataApi::ListMetadataRequest : public Request
{
public:
    virtual ~ListMetadataRequest() {}
	void SetupHttpRequest(const FHttpRequestRef& HttpRequest) const final;
	FString ComputePath() const final;

	/* The name of chain */
	FString ChainName;
	/* The address of metadata contract */
	FString ContractAddress;
	/* Datetime to use as the oldest updated timestamp */
	TOptional<FDateTime> FromUpdatedAt;
	/* Encoded page cursor to retrieve previous or next page. Use the value returned in the response. */
	TOptional<FString> PageCursor;
	/* Maximum number of items to return */
	TOptional<int32> PageSize;
};

class IMMUTABLEZKEVMAPI_API APIMetadataApi::ListMetadataResponse : public Response
{
public:
    virtual ~ListMetadataResponse() {}
	void SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode) final;
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;

    APIListMetadataResult Content;
};

/* Get a list of metadata from the given chain
 *
 * Get a list of metadata from the given chain
*/
class IMMUTABLEZKEVMAPI_API APIMetadataApi::ListMetadataForChainRequest : public Request
{
public:
    virtual ~ListMetadataForChainRequest() {}
	void SetupHttpRequest(const FHttpRequestRef& HttpRequest) const final;
	FString ComputePath() const final;

	/* The name of chain */
	FString ChainName;
	/* Datetime to use as the oldest updated timestamp */
	TOptional<FDateTime> FromUpdatedAt;
	/* Encoded page cursor to retrieve previous or next page. Use the value returned in the response. */
	TOptional<FString> PageCursor;
	/* Maximum number of items to return */
	TOptional<int32> PageSize;
};

class IMMUTABLEZKEVMAPI_API APIMetadataApi::ListMetadataForChainResponse : public Response
{
public:
    virtual ~ListMetadataForChainResponse() {}
	void SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode) final;
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;

    APIListMetadataResult Content;
};

/* List NFT stack bundles by stack_id. Response will include Market, Listings &amp; Stack Count information for each stack
 *
 * List NFT stack bundles by stack_id. This endpoint functions similarly to &#x60;ListMetadataByID&#x60; but extends the response to include Market, Listings &amp; Stack Count information for each stack.
*/
class IMMUTABLEZKEVMAPI_API APIMetadataApi::ListStacksRequest : public Request
{
public:
    virtual ~ListStacksRequest() {}
	void SetupHttpRequest(const FHttpRequestRef& HttpRequest) const final;
	FString ComputePath() const final;

	/* The name of chain */
	FString ChainName;
	/* List of stack_id to filter by */
	TArray<FGuid> StackId;
};

class IMMUTABLEZKEVMAPI_API APIMetadataApi::ListStacksResponse : public Response
{
public:
    virtual ~ListStacksResponse() {}
	void SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode) final;
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;

    TArray<APIStackBundle> Content;
};

/* Refresh stacked metadata
 *
 * Refresh stacked metadata
*/
class IMMUTABLEZKEVMAPI_API APIMetadataApi::RefreshMetadataByIDRequest : public Request
{
public:
    virtual ~RefreshMetadataByIDRequest() {}
	void SetupHttpRequest(const FHttpRequestRef& HttpRequest) const final;
	FString ComputePath() const final;

	/* The name of chain */
	FString ChainName;
	/* Contract address */
	FString ContractAddress;
	/* NFT Metadata Refresh Request */
	APIRefreshMetadataByIDRequest APIRefreshMetadataByIDRequest;
};

class IMMUTABLEZKEVMAPI_API APIMetadataApi::RefreshMetadataByIDResponse : public Response
{
public:
    virtual ~RefreshMetadataByIDResponse() {}
	void SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode) final;
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;

    APIMetadataRefreshRateLimitResult Content;
};

/* Refresh NFT metadata
 *
 * Refresh NFT metadata
*/
class IMMUTABLEZKEVMAPI_API APIMetadataApi::RefreshNFTMetadataByTokenIDRequest : public Request
{
public:
    virtual ~RefreshNFTMetadataByTokenIDRequest() {}
	void SetupHttpRequest(const FHttpRequestRef& HttpRequest) const final;
	FString ComputePath() const final;

	/* The address of contract */
	FString ContractAddress;
	/* The name of chain */
	FString ChainName;
	/* the request body */
	APIRefreshNFTMetadataByTokenIDRequest APIRefreshNFTMetadataByTokenIDRequest;
};

class IMMUTABLEZKEVMAPI_API APIMetadataApi::RefreshNFTMetadataByTokenIDResponse : public Response
{
public:
    virtual ~RefreshNFTMetadataByTokenIDResponse() {}
	void SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode) final;
	bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;

    APIMetadataRefreshRateLimitResult Content;
};

}