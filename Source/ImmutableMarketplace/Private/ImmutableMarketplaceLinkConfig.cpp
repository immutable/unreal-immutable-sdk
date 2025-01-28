#include "ImmutableMarketplaceLinkConfig.h"

#include "ImmutableEnums.h"

#include "ImmutableMarketplaceEnums.h"

FString UImmutableMarketplaceLinkConfig::OnRampBaseUrlSandbox = TEXT("https://global-stg.transak.com");
FString UImmutableMarketplaceLinkConfig::OnRampBaseUrlProduction = TEXT("https://global.transak.com");

FString UImmutableMarketplaceLinkConfig::SwapBaseUrlSandbox = TEXT("https://checkout-playground.sandbox.immutable.com/checkout/swap");
FString UImmutableMarketplaceLinkConfig::SwapBaseUrlProduction = TEXT("https://toolkit.immutable.com/checkout/swap");

FString UImmutableMarketplaceLinkConfig::BridgeBaseUrlSandbox = TEXT("https://checkout-playground.sandbox.immutable.com/checkout/squid");
FString UImmutableMarketplaceLinkConfig::BridgeBaseUrlProduction = TEXT("https://toolkit.immutable.com/checkout/squid");

// These can be hardcoded as they are public API keys
FString UImmutableMarketplaceLinkConfig::ApiOnRampKeySandbox = TEXT("d14b44fb-0f84-4db5-affb-e044040d724b");
FString UImmutableMarketplaceLinkConfig::ApiOnRampKeyProduction = TEXT("ad1bca70-d917-4628-bb0f-5609537498bc");

FString UImmutableMarketplaceLinkConfig::GetBaseUrl(EImmutableEnvironment Environment, EImmutableMarketplaceFlow Flow)
{
	switch (Flow)
	{
	case EImmutableMarketplaceFlow::OnRamp:
		return Environment == EImmutableEnvironment::Sandbox ? OnRampBaseUrlSandbox : OnRampBaseUrlProduction;

	case EImmutableMarketplaceFlow::Swap:
		return Environment == EImmutableEnvironment::Sandbox ? SwapBaseUrlSandbox : SwapBaseUrlProduction;

	case EImmutableMarketplaceFlow::Bridge:
		return Environment == EImmutableEnvironment::Sandbox ? BridgeBaseUrlSandbox : BridgeBaseUrlProduction;
	}

	return FString();
}

FString UImmutableMarketplaceLinkConfig::GetApiKey(EImmutableEnvironment Environment, EImmutableMarketplaceFlow Flow)
{
	return Flow == EImmutableMarketplaceFlow::OnRamp ? Environment == EImmutableEnvironment::Sandbox ? ApiOnRampKeySandbox : ApiOnRampKeyProduction : FString();
}