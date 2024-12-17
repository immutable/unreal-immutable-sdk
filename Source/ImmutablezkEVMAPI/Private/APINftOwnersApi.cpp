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

#include "APINftOwnersApi.h"

#include "APINftOwnersApiOperations.h"
#include "ImmutablezkEVMAPIModule.h"

#include "HttpModule.h"
#include "Serialization/JsonSerializer.h"

namespace ImmutablezkEVMAPI
{

APINftOwnersApi::APINftOwnersApi()
: Url(TEXT("https://api.sandbox.immutable.com"))
{
}

APINftOwnersApi::~APINftOwnersApi() {}

void APINftOwnersApi::SetURL(const FString& InUrl)
{
	Url = InUrl;
}

void APINftOwnersApi::AddHeaderParam(const FString& Key, const FString& Value)
{
	AdditionalHeaderParams.Add(Key, Value);
}

void APINftOwnersApi::ClearHeaderParams()
{
	AdditionalHeaderParams.Reset();
}

bool APINftOwnersApi::IsValid() const
{
	if (Url.IsEmpty())
	{
		UE_LOG(LogImmutablezkEVMAPI, Error, TEXT("APINftOwnersApi: Endpoint Url is not set, request cannot be performed"));
		return false;
	}

	return true;
}

void APINftOwnersApi::SetHttpRetryManager(FHttpRetrySystem::FManager& InRetryManager)
{
	if (RetryManager != &InRetryManager)
	{
		DefaultRetryManager.Reset();
		RetryManager = &InRetryManager;
	}
}

FHttpRetrySystem::FManager& APINftOwnersApi::GetHttpRetryManager()
{
	checkf(RetryManager, TEXT("APINftOwnersApi: RetryManager is null.  You may have meant to set it with SetHttpRetryManager first, or you may not be using a custom RetryManager at all."))
	return *RetryManager;
}

FHttpRequestRef APINftOwnersApi::CreateHttpRequest(const Request& Request) const
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

void APINftOwnersApi::HandleResponse(FHttpResponsePtr HttpResponse, bool bSucceeded, Response& InOutResponse) const
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

FHttpRequestPtr APINftOwnersApi::ListAllNFTOwners(const ListAllNFTOwnersRequest& Request, const FListAllNFTOwnersDelegate& Delegate /*= FListAllNFTOwnersDelegate()*/) const
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

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &APINftOwnersApi::OnListAllNFTOwnersResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void APINftOwnersApi::OnListAllNFTOwnersResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FListAllNFTOwnersDelegate Delegate) const
{
	ListAllNFTOwnersResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

FHttpRequestPtr APINftOwnersApi::ListNFTOwners(const ListNFTOwnersRequest& Request, const FListNFTOwnersDelegate& Delegate /*= FListNFTOwnersDelegate()*/) const
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

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &APINftOwnersApi::OnListNFTOwnersResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void APINftOwnersApi::OnListNFTOwnersResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FListNFTOwnersDelegate Delegate) const
{
	ListNFTOwnersResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

FHttpRequestPtr APINftOwnersApi::ListOwnersByContractAddress(const ListOwnersByContractAddressRequest& Request, const FListOwnersByContractAddressDelegate& Delegate /*= FListOwnersByContractAddressDelegate()*/) const
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

	HttpRequest->OnProcessRequestComplete().BindRaw(this, &APINftOwnersApi::OnListOwnersByContractAddressResponse, Delegate);
	HttpRequest->ProcessRequest();
	return HttpRequest;
}

void APINftOwnersApi::OnListOwnersByContractAddressResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FListOwnersByContractAddressDelegate Delegate) const
{
	ListOwnersByContractAddressResponse Response;
	HandleResponse(HttpResponse, bSucceeded, Response);
	Delegate.ExecuteIfBound(Response);
}

}