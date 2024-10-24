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

class IMMUTABLEZKEVMAPI_API APIPassportProfileApi
{
public:
	APIPassportProfileApi();
	~APIPassportProfileApi();

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

	class GetUserInfoRequest;
	class GetUserInfoResponse;
	class LinkWalletV2Request;
	class LinkWalletV2Response;
	
    DECLARE_DELEGATE_OneParam(FGetUserInfoDelegate, const GetUserInfoResponse&);
    DECLARE_DELEGATE_OneParam(FLinkWalletV2Delegate, const LinkWalletV2Response&);
    
    FHttpRequestPtr GetUserInfo(const GetUserInfoRequest& Request, const FGetUserInfoDelegate& Delegate = FGetUserInfoDelegate()) const;
    FHttpRequestPtr LinkWalletV2(const LinkWalletV2Request& Request, const FLinkWalletV2Delegate& Delegate = FLinkWalletV2Delegate()) const;
    
private:
    void OnGetUserInfoResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FGetUserInfoDelegate Delegate) const;
    void OnLinkWalletV2Response(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FLinkWalletV2Delegate Delegate) const;
    
	FHttpRequestRef CreateHttpRequest(const Request& Request) const;
	bool IsValid() const;
	void HandleResponse(FHttpResponsePtr HttpResponse, bool bSucceeded, Response& InOutResponse) const;

	FString Url;
	TMap<FString,FString> AdditionalHeaderParams;
	mutable FHttpRetrySystem::FManager* RetryManager = nullptr;
	mutable TUniquePtr<HttpRetryManager> DefaultRetryManager;
};

}
