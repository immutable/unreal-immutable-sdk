#include "Immutable/Marketplace/ImtblSwapRequest.h"

#include "GenericPlatform/GenericPlatformHttp.h"

FString FImtblSwapRequestQueryParams::GetPercentEncodedUrl() const
{
	return FString::Printf
	(
		TEXT("%s=%s&%s=%s&%s=%s"),
		TEXT("publishableKey"), *FGenericPlatformHttp::UrlEncode(PublishableKey),
		TEXT("fromTokenAddress"), *FGenericPlatformHttp::UrlEncode(FromTokenAddress),
		TEXT("toTokenAddress"), *FGenericPlatformHttp::UrlEncode(ToTokenAddress)
	);
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

bool UImtblSwapRequest::ComputePath(EImtblEnvironment Environment, FString FromTokenAddress, FString ToTokenAddress, FString& ComputedPath) const
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
		TEXT("%s?%s"),
		*BaseUrl, *QueryParamsPercentEncodedUrl
	);

	return true;
}