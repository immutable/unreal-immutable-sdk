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

#include "APICollectionsApiOperations.h"

#include "ImmutablezkEVMAPIModule.h"
#include "APIHelpers.h"

#include "Dom/JsonObject.h"
#include "Templates/SharedPointer.h"
#include "HttpModule.h"
#include "PlatformHttp.h"

namespace ImmutablezkEVMAPI
{

FString APICollectionsApi::GetCollectionRequest::ComputePath() const
{
	TMap<FString, FStringFormatArg> PathParams = { 
	{ TEXT("contract_address"), FStringFormatArg(ToUrlString(ContractAddress)) },
	{ TEXT("chain_name"), FStringFormatArg(ToUrlString(ChainName)) } };

	FString Path = FString::Format(TEXT("/v1/chains/{chain_name}/collections/{contract_address}"), PathParams);

	return Path;
}

void APICollectionsApi::GetCollectionRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = {  };
	//static const TArray<FString> Produces = { TEXT("application/json") };

	HttpRequest->SetVerb(TEXT("GET"));

}

void APICollectionsApi::GetCollectionResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT("200 response"));
		break;
	case 400:
		SetResponseString(TEXT("Bad Request (400)"));
		break;
	case 404:
		SetResponseString(TEXT("The specified resource was not found (404)"));
		break;
	case 500:
		SetResponseString(TEXT("Internal Server Error (500)"));
		break;
	}
}

bool APICollectionsApi::GetCollectionResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Content);
}

FString APICollectionsApi::ListCollectionsRequest::ComputePath() const
{
	TMap<FString, FStringFormatArg> PathParams = { 
	{ TEXT("chain_name"), FStringFormatArg(ToUrlString(ChainName)) } };

	FString Path = FString::Format(TEXT("/v1/chains/{chain_name}/collections"), PathParams);

	TArray<FString> QueryParams;
	if(ContractAddress.IsSet())
	{
		QueryParams.Add(CollectionToUrlString_multi(ContractAddress.GetValue(), TEXT("contract_address")));
	}
	if(VerificationStatus.IsSet())
	{
		QueryParams.Add(CollectionToUrlString_multi(VerificationStatus.GetValue(), TEXT("verification_status")));
	}
	if(FromUpdatedAt.IsSet())
	{
		QueryParams.Add(FString(TEXT("from_updated_at=")) + ToUrlString(FromUpdatedAt.GetValue()));
	}
	if(PageCursor.IsSet())
	{
		QueryParams.Add(FString(TEXT("page_cursor=")) + ToUrlString(PageCursor.GetValue()));
	}
	if(PageSize.IsSet())
	{
		QueryParams.Add(FString(TEXT("page_size=")) + ToUrlString(PageSize.GetValue()));
	}
	Path += TCHAR('?');
	Path += FString::Join(QueryParams, TEXT("&"));

	return Path;
}

void APICollectionsApi::ListCollectionsRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = {  };
	//static const TArray<FString> Produces = { TEXT("application/json") };

	HttpRequest->SetVerb(TEXT("GET"));

}

void APICollectionsApi::ListCollectionsResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT("200 response"));
		break;
	case 400:
		SetResponseString(TEXT("Bad Request (400)"));
		break;
	case 404:
		SetResponseString(TEXT("The specified resource was not found (404)"));
		break;
	case 500:
		SetResponseString(TEXT("Internal Server Error (500)"));
		break;
	}
}

bool APICollectionsApi::ListCollectionsResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Content);
}

FString APICollectionsApi::ListCollectionsByNFTOwnerRequest::ComputePath() const
{
	TMap<FString, FStringFormatArg> PathParams = { 
	{ TEXT("account_address"), FStringFormatArg(ToUrlString(AccountAddress)) },
	{ TEXT("chain_name"), FStringFormatArg(ToUrlString(ChainName)) } };

	FString Path = FString::Format(TEXT("/v1/chains/{chain_name}/accounts/{account_address}/collections"), PathParams);

	TArray<FString> QueryParams;
	if(PageCursor.IsSet())
	{
		QueryParams.Add(FString(TEXT("page_cursor=")) + ToUrlString(PageCursor.GetValue()));
	}
	if(PageSize.IsSet())
	{
		QueryParams.Add(FString(TEXT("page_size=")) + ToUrlString(PageSize.GetValue()));
	}
	Path += TCHAR('?');
	Path += FString::Join(QueryParams, TEXT("&"));

	return Path;
}

void APICollectionsApi::ListCollectionsByNFTOwnerRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = {  };
	//static const TArray<FString> Produces = { TEXT("application/json") };

	HttpRequest->SetVerb(TEXT("GET"));

}

void APICollectionsApi::ListCollectionsByNFTOwnerResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT("200 response"));
		break;
	case 400:
		SetResponseString(TEXT("Bad Request (400)"));
		break;
	case 404:
		SetResponseString(TEXT("The specified resource was not found (404)"));
		break;
	case 500:
		SetResponseString(TEXT("Internal Server Error (500)"));
		break;
	}
}

bool APICollectionsApi::ListCollectionsByNFTOwnerResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Content);
}

FString APICollectionsApi::RefreshCollectionMetadataRequest::ComputePath() const
{
	TMap<FString, FStringFormatArg> PathParams = { 
	{ TEXT("contract_address"), FStringFormatArg(ToUrlString(ContractAddress)) },
	{ TEXT("chain_name"), FStringFormatArg(ToUrlString(ChainName)) } };

	FString Path = FString::Format(TEXT("/v1/chains/{chain_name}/collections/{contract_address}/refresh-metadata"), PathParams);

	return Path;
}

void APICollectionsApi::RefreshCollectionMetadataRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = { TEXT("application/json") };
	//static const TArray<FString> Produces = { TEXT("application/json") };

	HttpRequest->SetVerb(TEXT("POST"));

	// Default to Json Body request
	if (Consumes.Num() == 0 || Consumes.Contains(TEXT("application/json")))
	{
		// Body parameters
		FString JsonBody;
		JsonWriter Writer = TJsonWriterFactory<>::Create(&JsonBody);

		WriteJsonValue(Writer, APIRefreshCollectionMetadataRequest);
		Writer->Close();

		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
		HttpRequest->SetContentAsString(JsonBody);
	}
	else if (Consumes.Contains(TEXT("multipart/form-data")))
	{
		UE_LOG(LogImmutablezkEVMAPI, Error, TEXT("Body parameter (APIRefreshCollectionMetadataRequest) was ignored, not supported in multipart form"));
	}
	else if (Consumes.Contains(TEXT("application/x-www-form-urlencoded")))
	{
		UE_LOG(LogImmutablezkEVMAPI, Error, TEXT("Body parameter (APIRefreshCollectionMetadataRequest) was ignored, not supported in urlencoded requests"));
	}
	else
	{
		UE_LOG(LogImmutablezkEVMAPI, Error, TEXT("Request ContentType not supported (%s)"), *FString::Join(Consumes, TEXT(",")));
	}
}

void APICollectionsApi::RefreshCollectionMetadataResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT("200 response"));
		break;
	case 400:
		SetResponseString(TEXT("Bad Request (400)"));
		break;
	case 401:
		SetResponseString(TEXT("Unauthorised Request (401)"));
		break;
	case 403:
		SetResponseString(TEXT("Forbidden Request (403)"));
		break;
	case 404:
		SetResponseString(TEXT("The specified resource was not found (404)"));
		break;
	case 500:
		SetResponseString(TEXT("Internal Server Error (500)"));
		break;
	}
}

bool APICollectionsApi::RefreshCollectionMetadataResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Content);
}

}
