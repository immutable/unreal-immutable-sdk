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

#include "CoreMinimal.h"
#include "APIBaseModel.h"

namespace ImmutablezkEVMAPI
{

class IMMUTABLEZKEVMAPI_API APIMetadataSearchApi
{
public:
	APIMetadataSearchApi();
	~APIMetadataSearchApi();

	/* Sets the URL Endpoint.
	* Note: several fallback endpoints can be configured in request retry policies, see Request::SetShouldRetry */
	void SetURL(const FString& Url);

	/* Adds global header params to all requests */
	void AddHeaderParam(const FString& Key, const FString& Value);
	void ClearHeaderParams();

	/* Sets the retry manager to the user-defined retry manager. User must manage the lifetime of the retry manager.
	* If no retry manager is specified and a request needs retries, a default retry manager will be used.
	* See also: Request::SetShouldRetry */
	void SetHttpRetryManager(FHttpRetrySystem::FManager& RetryManager);
	FHttpRetrySystem::FManager& GetHttpRetryManager();

	class ListFiltersRequest;
	class ListFiltersResponse;
	class SearchNFTsRequest;
	class SearchNFTsResponse;
	class SearchStacksRequest;
	class SearchStacksResponse;
	
    DECLARE_DELEGATE_OneParam(FListFiltersDelegate, const ListFiltersResponse&);
    DECLARE_DELEGATE_OneParam(FSearchNFTsDelegate, const SearchNFTsResponse&);
    DECLARE_DELEGATE_OneParam(FSearchStacksDelegate, const SearchStacksResponse&);
    
    FHttpRequestPtr ListFilters(const ListFiltersRequest& Request, const FListFiltersDelegate& Delegate = FListFiltersDelegate()) const;
    FHttpRequestPtr SearchNFTs(const SearchNFTsRequest& Request, const FSearchNFTsDelegate& Delegate = FSearchNFTsDelegate()) const;
    FHttpRequestPtr SearchStacks(const SearchStacksRequest& Request, const FSearchStacksDelegate& Delegate = FSearchStacksDelegate()) const;
    
private:
    void OnListFiltersResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FListFiltersDelegate Delegate) const;
    void OnSearchNFTsResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FSearchNFTsDelegate Delegate) const;
    void OnSearchStacksResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FSearchStacksDelegate Delegate) const;
    
	FHttpRequestRef CreateHttpRequest(const Request& Request) const;
	bool IsValid() const;
	void HandleResponse(FHttpResponsePtr HttpResponse, bool bSucceeded, Response& InOutResponse) const;

	FString Url;
	TMap<FString,FString> AdditionalHeaderParams;
	mutable FHttpRetrySystem::FManager* RetryManager = nullptr;
	mutable TUniquePtr<HttpRetryManager> DefaultRetryManager;
};

}