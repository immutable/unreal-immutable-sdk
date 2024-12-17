/**
 * TS SDK API
 * running ts sdk as an api
 *
 * OpenAPI spec version: 1.0.0
 * Contact: contact@immutable.com
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * https://github.com/OpenAPITools/openapi-generator
 * Do not edit the class manually.
 */

#include "APIOrderbookApiOperations.h"

#include "ImmutableOrderbookModule.h"
#include "APIHelpers.h"

#include "Dom/JsonObject.h"
#include "Templates/SharedPointer.h"
#include "HttpModule.h"
#include "PlatformHttp.h"

namespace ImmutableOrderbook
{

FString APIOrderbookApi::CancelOrdersRequest::ComputePath() const
{
	FString Path(TEXT("/v1/ts-sdk/orderbook/cancelOrders"));
	return Path;
}

void APIOrderbookApi::CancelOrdersRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = { TEXT("application/json") };
	//static const TArray<FString> Produces = { TEXT("application/json") };

	HttpRequest->SetVerb(TEXT("POST"));

	// Default to Json Body request
	if (Consumes.Num() == 0 || Consumes.Contains(TEXT("application/json")))
	{
		// Body parameters
		FString JsonBody;
		JsonWriter Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonBody);

		if (APICancelOrdersRequest.IsSet())
		{
			WriteJsonValue(Writer, APICancelOrdersRequest.GetValue());
		}
		Writer->Close();

		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
		HttpRequest->SetContentAsString(JsonBody);
	}
	else if (Consumes.Contains(TEXT("multipart/form-data")))
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Body parameter (APICancelOrdersRequest) was ignored, not supported in multipart form"));
	}
	else if (Consumes.Contains(TEXT("application/x-www-form-urlencoded")))
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Body parameter (APICancelOrdersRequest) was ignored, not supported in urlencoded requests"));
	}
	else
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Request ContentType not supported (%s)"), *FString::Join(Consumes, TEXT(",")));
	}
}

void APIOrderbookApi::CancelOrdersResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT("Response schema for the cancelOrder endpoint"));
		break;
	}
}

bool APIOrderbookApi::CancelOrdersResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Content);
}

FString APIOrderbookApi::CancelOrdersOnChainRequest::ComputePath() const
{
	FString Path(TEXT("/v1/ts-sdk/orderbook/cancelOrdersOnChain"));
	return Path;
}

void APIOrderbookApi::CancelOrdersOnChainRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = { TEXT("application/json") };
	//static const TArray<FString> Produces = { TEXT("application/json") };

	HttpRequest->SetVerb(TEXT("POST"));

	// Default to Json Body request
	if (Consumes.Num() == 0 || Consumes.Contains(TEXT("application/json")))
	{
		// Body parameters
		FString JsonBody;
		JsonWriter Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonBody);

		if (APICancelOrdersOnChainRequest.IsSet())
		{
			WriteJsonValue(Writer, APICancelOrdersOnChainRequest.GetValue());
		}
		Writer->Close();

		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
		HttpRequest->SetContentAsString(JsonBody);
	}
	else if (Consumes.Contains(TEXT("multipart/form-data")))
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Body parameter (APICancelOrdersOnChainRequest) was ignored, not supported in multipart form"));
	}
	else if (Consumes.Contains(TEXT("application/x-www-form-urlencoded")))
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Body parameter (APICancelOrdersOnChainRequest) was ignored, not supported in urlencoded requests"));
	}
	else
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Request ContentType not supported (%s)"), *FString::Join(Consumes, TEXT(",")));
	}
}

void APIOrderbookApi::CancelOrdersOnChainResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT("Response schema for the cancelOrder endpoint"));
		break;
	}
}

bool APIOrderbookApi::CancelOrdersOnChainResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Content);
}

FString APIOrderbookApi::CreateListingRequest::ComputePath() const
{
	FString Path(TEXT("/v1/ts-sdk/orderbook/createListing"));
	return Path;
}

void APIOrderbookApi::CreateListingRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = { TEXT("application/json") };
	//static const TArray<FString> Produces = { TEXT("application/json") };

	HttpRequest->SetVerb(TEXT("POST"));

	// Default to Json Body request
	if (Consumes.Num() == 0 || Consumes.Contains(TEXT("application/json")))
	{
		// Body parameters
		FString JsonBody;
		JsonWriter Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonBody);

		if (APICreateListingRequest.IsSet())
		{
			WriteJsonValue(Writer, APICreateListingRequest.GetValue());
		}
		Writer->Close();

		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
		HttpRequest->SetContentAsString(JsonBody);
	}
	else if (Consumes.Contains(TEXT("multipart/form-data")))
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Body parameter (APICreateListingRequest) was ignored, not supported in multipart form"));
	}
	else if (Consumes.Contains(TEXT("application/x-www-form-urlencoded")))
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Body parameter (APICreateListingRequest) was ignored, not supported in urlencoded requests"));
	}
	else
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Request ContentType not supported (%s)"), *FString::Join(Consumes, TEXT(",")));
	}
}

void APIOrderbookApi::CreateListingResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT("The response schema for the create listing endpoint"));
		break;
	}
}

bool APIOrderbookApi::CreateListingResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Content);
}

FString APIOrderbookApi::FulfillOrderRequest::ComputePath() const
{
	FString Path(TEXT("/v1/ts-sdk/orderbook/fulfillOrder"));
	return Path;
}

void APIOrderbookApi::FulfillOrderRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = { TEXT("application/json") };
	//static const TArray<FString> Produces = { TEXT("application/json") };

	HttpRequest->SetVerb(TEXT("POST"));

	// Default to Json Body request
	if (Consumes.Num() == 0 || Consumes.Contains(TEXT("application/json")))
	{
		// Body parameters
		FString JsonBody;
		JsonWriter Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonBody);

		if (APIFulfillOrderRequest.IsSet())
		{
			WriteJsonValue(Writer, APIFulfillOrderRequest.GetValue());
		}
		Writer->Close();

		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
		HttpRequest->SetContentAsString(JsonBody);
	}
	else if (Consumes.Contains(TEXT("multipart/form-data")))
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Body parameter (APIFulfillOrderRequest) was ignored, not supported in multipart form"));
	}
	else if (Consumes.Contains(TEXT("application/x-www-form-urlencoded")))
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Body parameter (APIFulfillOrderRequest) was ignored, not supported in urlencoded requests"));
	}
	else
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Request ContentType not supported (%s)"), *FString::Join(Consumes, TEXT(",")));
	}
}

void APIOrderbookApi::FulfillOrderResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT("Response schema for the fulfillOrder endpoint"));
		break;
	}
}

bool APIOrderbookApi::FulfillOrderResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Content);
}

FString APIOrderbookApi::PrepareListingRequest::ComputePath() const
{
	FString Path(TEXT("/v1/ts-sdk/orderbook/prepareListing"));
	return Path;
}

void APIOrderbookApi::PrepareListingRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = { TEXT("application/json") };
	//static const TArray<FString> Produces = { TEXT("application/json") };

	HttpRequest->SetVerb(TEXT("POST"));

	// Default to Json Body request
	if (Consumes.Num() == 0 || Consumes.Contains(TEXT("application/json")))
	{
		// Body parameters
		FString JsonBody;
		JsonWriter Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonBody);

		if (APIPrepareListingRequest.IsSet())
		{
			WriteJsonValue(Writer, APIPrepareListingRequest.GetValue());
		}
		Writer->Close();

		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
		HttpRequest->SetContentAsString(JsonBody);
	}
	else if (Consumes.Contains(TEXT("multipart/form-data")))
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Body parameter (APIPrepareListingRequest) was ignored, not supported in multipart form"));
	}
	else if (Consumes.Contains(TEXT("application/x-www-form-urlencoded")))
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Body parameter (APIPrepareListingRequest) was ignored, not supported in urlencoded requests"));
	}
	else
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Request ContentType not supported (%s)"), *FString::Join(Consumes, TEXT(",")));
	}
}

void APIOrderbookApi::PrepareListingResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT("Response schema for the prepareListing endpoint"));
		break;
	}
}

bool APIOrderbookApi::PrepareListingResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Content);
}

FString APIOrderbookApi::PrepareOrderCancellationsRequest::ComputePath() const
{
	FString Path(TEXT("/v1/ts-sdk/orderbook/prepareOrderCancellations"));
	return Path;
}

void APIOrderbookApi::PrepareOrderCancellationsRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = { TEXT("application/json") };
	//static const TArray<FString> Produces = { TEXT("application/json") };

	HttpRequest->SetVerb(TEXT("POST"));

	// Default to Json Body request
	if (Consumes.Num() == 0 || Consumes.Contains(TEXT("application/json")))
	{
		// Body parameters
		FString JsonBody;
		JsonWriter Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonBody);

		if (APIPrepareOrderCancellationsRequest.IsSet())
		{
			WriteJsonValue(Writer, APIPrepareOrderCancellationsRequest.GetValue());
		}
		Writer->Close();

		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
		HttpRequest->SetContentAsString(JsonBody);
	}
	else if (Consumes.Contains(TEXT("multipart/form-data")))
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Body parameter (APIPrepareOrderCancellationsRequest) was ignored, not supported in multipart form"));
	}
	else if (Consumes.Contains(TEXT("application/x-www-form-urlencoded")))
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Body parameter (APIPrepareOrderCancellationsRequest) was ignored, not supported in urlencoded requests"));
	}
	else
	{
		UE_LOG(LogImmutableOrderbook, Error, TEXT("Request ContentType not supported (%s)"), *FString::Join(Consumes, TEXT(",")));
	}
}

void APIOrderbookApi::PrepareOrderCancellationsResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT("Response schema for the prepareOrderCancellations endpoint"));
		break;
	}
}

bool APIOrderbookApi::PrepareOrderCancellationsResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Content);
}

FString APIOrderbookApi::TokenBalanceRequest::ComputePath() const
{
	FString Path(TEXT("/v1/ts-sdk/token/balance"));
	TArray<FString> QueryParams;
	QueryParams.Add(FString(TEXT("wallet_address=")) + ToUrlString(WalletAddress));
	QueryParams.Add(FString(TEXT("contract_address=")) + ToUrlString(ContractAddress));
	Path += TCHAR('?');
	Path += FString::Join(QueryParams, TEXT("&"));

	return Path;
}

void APIOrderbookApi::TokenBalanceRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
{
	static const TArray<FString> Consumes = {  };
	//static const TArray<FString> Produces = { TEXT("application/json") };

	HttpRequest->SetVerb(TEXT("GET"));

}

void APIOrderbookApi::TokenBalanceResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
{
	Response::SetHttpResponseCode(InHttpResponseCode);
	switch ((int)InHttpResponseCode)
	{
	case 200:
		SetResponseString(TEXT("The response body returned from get balance endpoint"));
		break;
	}
}

bool APIOrderbookApi::TokenBalanceResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
{
	return TryGetJsonValue(JsonValue, Content);
}

}