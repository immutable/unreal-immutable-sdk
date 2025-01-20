#include "Immutable/Marketplace/ImtblSwapRequest.h"

#include "GenericPlatform/GenericPlatformHttp.h"

FString FImtblSwapRequestQueryParams::GetPercentEncodedUrl() const
{
	TArray<FString> QueryParams;

	if (!PublishableKey.IsEmpty())
	{
		QueryParams.Add(FString::Printf(TEXT("%s=%s"), TEXT("publishableKey"), *FGenericPlatformHttp::UrlEncode(PublishableKey)));
	}

	if (!FromTokenAddress.IsEmpty())
	{
		QueryParams.Add(FString::Printf(TEXT("%s=%s"), TEXT("fromTokenAddress"), *FGenericPlatformHttp::UrlEncode(FromTokenAddress)));
	}

	if (!ToTokenAddress.IsEmpty())
	{
		QueryParams.Add(FString::Printf(TEXT("%s=%s"), TEXT("toTokenAddress"), *FGenericPlatformHttp::UrlEncode(ToTokenAddress)));
	}

	return FString::Join(QueryParams, TEXT("&"));
}

bool UImtblSwapRequest::GetBaseUrl(EImtblEnvironment Environment, FString& BaseUrl) const
{
	if (const FString* Find = BaseUrls.Find(Environment))
	{
		BaseUrl = *Find;

		return true;
	}

	return true;
}

bool UImtblSwapRequest::GetApiKey(EImtblEnvironment Environment, FString& ApiKey) const
{
	if (const FString* Find = ApiKeys.Find(Environment))
	{
		ApiKey = *Find;

		return true;
	}

	return false;
}

bool UImtblSwapRequest::ComputePath(FString& ComputedPath, EImtblEnvironment Environment, FString FromTokenAddress, FString ToTokenAddress) const
{
	FString BaseUrl;
	bool bFoundBaseUrl = GetBaseUrl(Environment, BaseUrl);

	if (!bFoundBaseUrl)
	{
		return false;
	}

	FString ApiKey;
	bool bFoundApiKey = GetApiKey(Environment, ApiKey);

	if (!bFoundApiKey)
	{
		return false;
	}

	FImtblSwapRequestQueryParams QueryParams;
	QueryParams.PublishableKey = ApiKey;
	QueryParams.FromTokenAddress = FromTokenAddress;
	QueryParams.ToTokenAddress = ToTokenAddress;

	const FString QueryParamsPercentEncodedUrl = QueryParams.GetPercentEncodedUrl();

	ComputedPath = FString::Printf
	(
		TEXT("%s%s%s"),
		*BaseUrl,
		!QueryParamsPercentEncodedUrl.IsEmpty() ? TEXT("?") : TEXT(""),
		*QueryParamsPercentEncodedUrl
	);

	return true;
}