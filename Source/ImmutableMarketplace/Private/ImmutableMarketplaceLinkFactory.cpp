#include "ImmutableMarketplaceLinkFactory.h"

#include "PlatformHttp.h"

#include "ImmutableMarketplaceEnums.h"
#include "ImmutableMarketplaceLinkConfig.h"

FString UImmutableMarketplaceLinkFactory::GenerateOnRampLink(EImmutableEnvironment Environment, FString Email, FString WalletAddress, const FImmutableOnRampQueryParams& QueryParams, TMap<FString, FString> ExtraQueryParams)
{
	auto FindExtraQueryParam = [&](const FString& Key, const FString& DefaultValue) -> FString
	{
		if (FString* Value = ExtraQueryParams.Find(Key))
		{
			return *Value;
		}

		return DefaultValue;
	};

	const FString BaseUrl = UImmutableMarketplaceLinkConfig::GetBaseUrl(Environment, EImmutableMarketplaceFlow::OnRamp);
	const FString ApiKey = UImmutableMarketplaceLinkConfig::GetApiKey(Environment, EImmutableMarketplaceFlow::OnRamp);

	TArray<FString> QueryParamsArray
	{
		FString::Printf(TEXT("%s=%s"), TEXT("apiKey"), *FPlatformHttp::UrlEncode(ApiKey)),
		FString::Printf(TEXT("%s=%s"), TEXT("cryptoCurrencyList"), *FPlatformHttp::UrlEncode(QueryParams.CryptoCurrencyList)),
		FString::Printf(TEXT("%s=%s"), TEXT("defaultCryptoCurrency"), *FPlatformHttp::UrlEncode(QueryParams.DefaultCryptoCurrency)),
		FString::Printf(TEXT("%s=%s"), TEXT("defaultFiatAmount"), *FPlatformHttp::UrlEncode(QueryParams.DefaultFiatAmount)),
		FString::Printf(TEXT("%s=%s"), TEXT("defaultFiatCurrency"), *FPlatformHttp::UrlEncode(QueryParams.DefaultFiatCurrency)),
		FString::Printf(TEXT("%s=%s"), TEXT("defaultPaymentMethod"), *FPlatformHttp::UrlEncode(FindExtraQueryParam(TEXT("defaultPaymentMethod"), TEXT("credit_debit_card")))),
		FString::Printf(TEXT("%s=%s"), TEXT("disablePaymentMethods"), *FPlatformHttp::UrlEncode(FindExtraQueryParam("disablePaymentMethods", ""))),
		FString::Printf(TEXT("%s=%s"), TEXT("disableWalletAddressForm"), *FPlatformHttp::UrlEncode(FindExtraQueryParam(TEXT("disableWalletAddressForm"), TEXT("true")))),
		FString::Printf(TEXT("%s=%s"), TEXT("email"), *FPlatformHttp::UrlEncode(Email)),
		FString::Printf(TEXT("%s=%s"), TEXT("exchangeScreenTitle"), *FPlatformHttp::UrlEncode(FindExtraQueryParam("exchangeScreenTitle", TEXT("Buy")))),
		FString::Printf(TEXT("%s=%s"), TEXT("isAutoFillUserData"), *FPlatformHttp::UrlEncode(FindExtraQueryParam(TEXT("isAutoFillUserData"), TEXT("true")))),
		FString::Printf(TEXT("%s=%s"), TEXT("network"), TEXT("immutablezkevm")),
		FString::Printf(TEXT("%s=%s"), TEXT("productsAvailed"), TEXT("buy")),
		FString::Printf(TEXT("%s=%s"), TEXT("themeColor"), *FPlatformHttp::UrlEncode(FindExtraQueryParam(TEXT("themeColor"), TEXT("0D0D0D")))),
		FString::Printf(TEXT("%s=%s"), TEXT("walletAddress"), *FPlatformHttp::UrlEncode(WalletAddress))
	};

	for (TTuple<FString, FString> ExtraQueryParam : ExtraQueryParams)
	{
		QueryParamsArray.AddUnique(FString::Printf(TEXT("%s=%s"), *ExtraQueryParam.Key, *FPlatformHttp::UrlEncode(ExtraQueryParam.Value)));
	}

	FString QueryString = FString::Join(QueryParamsArray, TEXT("&"));

	return FString::Printf(TEXT("%s?%s"), *BaseUrl, *QueryString);
}

FString UImmutableMarketplaceLinkFactory::GenerateSwapLink(EImmutableEnvironment Environment, FString PublishableKey, const FImmutableSwapQueryParams& QueryParams)
{
	const FString BaseUrl = UImmutableMarketplaceLinkConfig::GetBaseUrl(Environment, EImmutableMarketplaceFlow::Swap);

	TArray<FString> QueryParamsArray;

	if (!PublishableKey.IsEmpty())
	{
		QueryParamsArray.Add(FString::Printf(TEXT("%s=%s"), TEXT("publishableKey"), *FPlatformHttp::UrlEncode(PublishableKey)));
	};

	if (!QueryParams.FromTokenAddress.IsEmpty())
	{
		QueryParamsArray.Add(FString::Printf(TEXT("%s=%s"), TEXT("fromTokenAddress"), *FPlatformHttp::UrlEncode(QueryParams.FromTokenAddress)));
	}

	if (!QueryParams.ToTokenAddress.IsEmpty())
	{
		QueryParamsArray.Add(FString::Printf(TEXT("%s=%s"), TEXT("toTokenAddress"), *FPlatformHttp::UrlEncode(QueryParams.ToTokenAddress)));
	}

	FString QueryString = FString::Join(QueryParamsArray, TEXT("&"));

	return FString::Printf(TEXT("%s?%s"), *BaseUrl, *QueryString);
}

FString UImmutableMarketplaceLinkFactory::GenerateBridgeLink(EImmutableEnvironment Environment, const FImmutableBridgeQueryParams& QueryParams)
{
	const FString BaseUrl = UImmutableMarketplaceLinkConfig::GetBaseUrl(Environment, EImmutableMarketplaceFlow::Bridge);

	TArray<FString> QueryParamsArray;

	if (!QueryParams.FromChainID.IsEmpty())
	{
		QueryParamsArray.Add(FString::Printf(TEXT("%s=%s"),TEXT("fromChain"), *FPlatformHttp::UrlEncode(QueryParams.FromChainID)));
	}

	if (!QueryParams.FromTokenAddress.IsEmpty())
	{
		// Token address is case-sensitive.
		QueryParamsArray.Add(FString::Printf(TEXT("%s=%s"), TEXT("fromToken"), *FPlatformHttp::UrlEncode(QueryParams.FromTokenAddress.ToLower())));
	}

	if (!QueryParams.ToChainID.IsEmpty())
	{
		QueryParamsArray.Add(FString::Printf(TEXT("%s=%s"),TEXT("toChain"), *FPlatformHttp::UrlEncode(QueryParams.ToChainID)));
	}

	if (!QueryParams.ToTokenAddress.IsEmpty())
	{
		// Token address is case-sensitive.
		QueryParamsArray.Add(FString::Printf(TEXT("%s=%s"),TEXT("toToken"), *FPlatformHttp::UrlEncode(QueryParams.ToTokenAddress.ToLower())));
	}

	FString QueryString = FString::Join(QueryParamsArray, TEXT("&"));

	return FString::Printf(TEXT("%s?%s"), *BaseUrl, *QueryString);
}