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

#include "APIMetadataSearchApi.h"

#include "APIMetadataSearchApiOperations.h"
#include "ImmutablezkEVMAPIModule.h"

#include "HttpModule.h"
#include "Serialization/JsonSerializer.h"

namespace ImmutablezkEVMAPI
{

APIMetadataSearchApi::APIMetadataSearchApi()
: Url(TEXT("https://api.sandbox.immutable.com"))
{
}

APIMetadataSearchApi::~APIMetadataSearchApi() {}

void APIMetadataSearchApi::SetURL(const FString& InUrl)
{
	Url = InUrl;
}

void APIMetadataSearchApi::AddHeaderParam(const FString& Key, const FString& Value)
{
	AdditionalHeaderParams.Add(Key, Value);
}

void APIMetadataSearchApi::ClearHeaderParams()
{
	AdditionalHeaderParams.Reset();
}

bool APIMetadataSearchApi::IsValid() const
{
	if (Url.IsEmpty())
	{
		UE_LOG(LogImmutablezkEVMAPI, Error, TEXT("APIMetadataSearchApi: Endpoint Url is not set, request cannot be performed"));
		return false;
	}

	return true;
}

void APIMetadataSearchApi::SetHttpRetryManager(FHttpRetrySystem::FManager& InRetryManager)
{
	if (RetryManager != &InRetryManager)
	{
		DefaultRetryManager.Reset();
		RetryManager = &InRetryManager;
	}
}

FHttpRetrySystem::FManager& APIMetadataSearchApi::GetHttpRetryManager()
{
	checkf(RetryManager, TEXT("APIMetadataSearchApi: RetryManager is null.  You may have meant to set it with SetHttpRetryManager first, or you may not be using a custom RetryManager at all."))
	return *RetryManager;
}

FHttpRequestRef APIMetadataSearchApi::CreateHttpRequest(const Request& Request) const
{
	if (!Request.GetRetryParams().IsSet())
	{
		return FHttpModule::Get().CreateRequest();
	}
	else
	{
		if (!RetryManager)
		{
			// Create default retry manager if none was specified
			DefaultRetryManager = MakeUnique<HttpRetryManager>(6, 60);
			RetryManager = DefaultRetryManager.Get();
		}

		const HttpRetryParams& Params = Request.GetRetryParams().GetValue();
		return RetryManager->CreateRequest(Params.RetryLimitCountOverride, Params.RetryTimeoutRelativeSecondsOverride, Params.RetryResponseCodes, Params.RetryVerbs, Params.RetryDomains);
	}
}

void APIMetadataSearchApi::HandleResponse(FHttpResponsePtr HttpResponse, bool bSucceeded, Response& InOutResponse) const
{
	InOutResponse.SetHttpResponse(HttpResponse);
	InOutResponse.SetSuccessful(bSucceeded);

	if (bSucceeded && HttpResponse.IsValid())
	{
		InOutResponse.SetHttpResponseCode((EHttpResponseCodes::Type)HttpResponse->GetResponseCode());
		FString ContentType = HttpResponse->GetContentType();
		FString Content;

		if (ContentType.IsEmpty())
		{
			return; // Nothing to parse
		}
		else if (ContentType.StartsWith(TEXT("application/json")) || ContentType.StartsWith("text/json"))
		{
			Content = HttpResponse->GetContentAsString();

			TSharedPtr<FJsonValue> JsonValue;
			auto Reader = TJsonReaderFactory<>::Create(Content);

			if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid())
			{
				if (InOutResponse.FromJson(JsonValue))
					return; // Successfully parsed
			}
		}
		else if(ContentType.StartsWith(TEXT("text/plain")))
		{
			Content = HttpResponse->GetContentAsString();
			InOutResponse.SetResponseString(Content);
			return; // Successfully parsed
		}

		// Report the parse error but do not mark the request as unsuccessful. Data could be partial or malformed, but the request succeeded.
		UE_LOG(LogImmutablezkEVMAPI, Error, TEXT("Failed to deserialize Http response content (type:%s):\n%s"), *ContentType , *Content);
		return;
	}

	// By default, assume we failed to establish connection
	InOutResponse.SetHttpResponseCode(EHttpResponseCodes::RequestTimeout);
}

FHttpRequestPtr APIMetadataSearchApi::ListFilters(const ListFiltersRequest& Request, const FListFiltersDelegate& Delegate /*= FListFiltersDelegate()*/) const
{
	if (!IsValid())
		return nullptr;

	FHttpRequestRef HttpRequest = CreateHttpRequest(Request);
	HttpRequest->SetURL(*(Url + Request.ComputePath()));

	for(const auto& It : AdditionalHeaderParams)
	{
		HttpRequest->SetHeader(It.Key, It.Value);
	}

	Request.SetupHttpRequest(HttpRequest);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &APIMetadataSearchApi::OnListFiltersResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void APIMetadataSearchApi::OnListFiltersResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FListFiltersDelegate Delegate) const
{
	ListFiltersResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

FHttpRequestPtr APIMetadataSearchApi::SearchNFTs(const SearchNFTsRequest& Request, const FSearchNFTsDelegate& Delegate /*= FSearchNFTsDelegate()*/) const
{
	if (!IsValid())
		return nullptr;

	FHttpRequestRef HttpRequest = CreateHttpRequest(Request);
	HttpRequest->SetURL(*(Url + Request.ComputePath()));

	for(const auto& It : AdditionalHeaderParams)
	{
		HttpRequest->SetHeader(It.Key, It.Value);
	}

	Request.SetupHttpRequest(HttpRequest);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &APIMetadataSearchApi::OnSearchNFTsResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void APIMetadataSearchApi::OnSearchNFTsResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FSearchNFTsDelegate Delegate) const
{
	SearchNFTsResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

FHttpRequestPtr APIMetadataSearchApi::SearchStacks(const SearchStacksRequest& Request, const FSearchStacksDelegate& Delegate /*= FSearchStacksDelegate()*/) const
{
	if (!IsValid())
		return nullptr;

	FHttpRequestRef HttpRequest = CreateHttpRequest(Request);
	HttpRequest->SetURL(*(Url + Request.ComputePath()));

	for(const auto& It : AdditionalHeaderParams)
	{
		HttpRequest->SetHeader(It.Key, It.Value);
	}

	Request.SetupHttpRequest(HttpRequest);

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &APIMetadataSearchApi::OnSearchStacksResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void APIMetadataSearchApi::OnSearchStacksResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FSearchStacksDelegate Delegate) const
{
	SearchStacksResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

}