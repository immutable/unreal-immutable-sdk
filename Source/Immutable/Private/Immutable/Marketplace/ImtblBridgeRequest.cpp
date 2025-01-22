#include "Immutable/Marketplace/ImtblBridgeRequest.h"

#include "GenericPlatform/GenericPlatformHttp.h"

FString FImtblBridgeRequestQueryParams::GetPercentEncodedUrl() const
{
	TArray<FString> QueryParams;

	if (!FromChainID.IsEmpty())
	{
		QueryParams.Add(FString::Printf(TEXT("%s=%s"), TEXT("fromChain"), *FGenericPlatformHttp::UrlEncode(FromChainID)));
	}

	if (!FromTokenAddress.IsEmpty())
	{
		// Token address is case-sensitive.
		QueryParams.Add(FString::Printf(TEXT("%s=%s"), TEXT("fromToken"), *FGenericPlatformHttp::UrlEncode(FromTokenAddress.ToLower())));
	}

	if (!ToChainID.IsEmpty())
	{
		QueryParams.Add(FString::Printf(TEXT("%s=%s"), TEXT("toChain"), *FGenericPlatformHttp::UrlEncode(ToChainID)));
	}

	if (!ToTokenAddress.IsEmpty())
	{
		// Token address is case-sensitive.
		QueryParams.Add(FString::Printf(TEXT("%s=%s"), TEXT("toToken"), *FGenericPlatformHttp::UrlEncode(ToTokenAddress.ToLower())));
	}

	return FString::Join(QueryParams, TEXT("&"));
}

bool UImtblBridgeRequest::GetBaseUrl(EImtblEnvironment Environment, FString& BaseUrl) const
{
	if (const FString* Find = BaseUrls.Find(Environment))
	{
		BaseUrl = *Find;

		return true;
	}

	return true;
}

bool UImtblBridgeRequest::ComputePath(FString& ComputedPath, EImtblEnvironment Environment, FString FromChainID, FString FromTokenAddress, FString ToChainID, FString ToTokenAddress) const
{
	FString BaseUrl;
	bool bFoundBaseUrl = GetBaseUrl(Environment, BaseUrl);

	if (!bFoundBaseUrl)
	{
		return false;
	}

	FImtblBridgeRequestQueryParams QueryParams;
	QueryParams.FromChainID = FromChainID;
	QueryParams.FromTokenAddress = FromTokenAddress;
	QueryParams.ToChainID = ToChainID;
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