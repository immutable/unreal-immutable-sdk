#include "OnRampWebBrowserWidget.h"

#include "ImmutableMarketplaceSettings.h"
#include "PlatformHttp.h"

#include "OnRampWidgetConfig.h"

#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
#include "SWebBrowser.h"
#else
#include "UserInterface/BluWebBrowser.h"
#endif

#define LOCTEXT_NAMESPACE "OnRampWidget"

DEFINE_LOG_CATEGORY(LogImmutableOnRampWidget);


bool UOnRampWidget::IsReady() const
{
	return bIsReady;
}

void UOnRampWidget::Load(const FString& WalletAddress, const FString& Email, const FString& ProductsAvailed, const FString& ScreenTitle)
{
	if (!WebBrowserWidget.IsValid())
	{
		return;
	}

	FString UrlToLoad = ComputePath(WalletAddress, Email, ProductsAvailed, ScreenTitle);
	CallAndRegister_OnWhenReady(UOnRampWidget::FOnWhenReady::FDelegate::CreateWeakLambda(this, [this, UrlToLoad]()
	{
		WebBrowserWidget->LoadURL(UrlToLoad);
		OnWhenReady.RemoveAll(this);
	}));
}

FDelegateHandle UOnRampWidget::CallAndRegister_OnWhenReady(FOnWhenReady::FDelegate Delegate)
{
	if (bIsReady)
	{
		Delegate.ExecuteIfBound();
	}

	return OnWhenReady.Add(Delegate);
}

TSharedRef<SWidget> UOnRampWidget::RebuildWidget()
{
	if (IsDesignTime())
	{
		return SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("OnRamp Web Browser", "OnRamp Web Browser"))
			];
	}
	else
	{
#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
		WebBrowserWidget = SNew(SWebBrowser)
			.InitialURL(InitialURL)
			.ShowControls(false)
			.SupportsTransparency(false)
			.OnUrlChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnUrlChanged))
			.OnBeforePopup(BIND_UOBJECT_DELEGATE(FOnBeforePopupDelegate, HandleOnBeforePopup))
			.OnConsoleMessage(BIND_UOBJECT_DELEGATE(FOnConsoleMessageDelegate, HandleOnConsoleMessage));
		return WebBrowserWidget.ToSharedRef();
#else
		WebBrowserWidget = SNew(SBluWebBrowser)
			.InitialURL(InitialURL)
			.OnUrlChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnUrlChanged));
		return WebBrowserWidget.ToSharedRef();
#endif
	}
}

FString UOnRampWidget::ComputePath(const FString& WalletAddress, const FString& Email, const FString& ProductsAvailed, const FString& ScreenTitle)
{
	auto Settings = GetDefault<UImmutableMarketplaceSettings>();

	UOnRampWidgetConfig* OnRampWidgetConfig = Settings->DefaultOnRampWidgetConfig.GetDefaultObject();

	if (!OnRampWidgetConfig)
	{
		UE_LOG(LogImmutableOnRampWidget, Error, TEXT("On ramp widget config is not assigned!"));
		
		return TEXT("");
	}

	FString Path = OnRampWidgetConfig->GetURL();
	TArray<FString> QueryParams;

	QueryParams.Add(FString(TEXT("apiKey=")) + FPlatformHttp::UrlEncode(OnRampWidgetConfig->GetAPIKey()));
	QueryParams.Add(FString(TEXT("email=")) + FPlatformHttp::UrlEncode(Email));
	QueryParams.Add(FString(TEXT("walletAddress=")) + FPlatformHttp::UrlEncode(WalletAddress));
	QueryParams.Add(FString(TEXT("themeColor=")) + FPlatformHttp::UrlEncode(OnRampWidgetConfig->GetThemeColor().ToString()));
	QueryParams.Add(FString(TEXT("isAutoFillUserData=")) + FPlatformHttp::UrlEncode(OnRampWidgetConfig->IsAutoFillUserData() ? TEXT("true") : TEXT("false")));
	QueryParams.Add(FString(TEXT("disableWalletAddressForm=")) + FPlatformHttp::UrlEncode(OnRampWidgetConfig->DisableWalletAddressForm() ? TEXT("true") : TEXT("false")));

	if (!OnRampWidgetConfig->GetNetwork().IsEmpty())
	{
		QueryParams.Add(FString(TEXT("network=")) + FPlatformHttp::UrlEncode(OnRampWidgetConfig->GetNetwork()));
	}

	if (!ProductsAvailed.IsEmpty())
	{
		QueryParams.Add(FString(TEXT("productsAvailed=")) + FPlatformHttp::UrlEncode(ProductsAvailed));
	}

	if (!ScreenTitle.IsEmpty())
	{
		QueryParams.Add(FString(TEXT("exchangeScreenTitle=")) + FPlatformHttp::UrlEncode(ScreenTitle));
	}

	if (!OnRampWidgetConfig->GetDefaultCryptoCurrency().IsEmpty())
	{
		QueryParams.Add(FString(TEXT("defaultCryptoCurrency=")) + FPlatformHttp::UrlEncode(OnRampWidgetConfig->GetDefaultCryptoCurrency()));
	}

	if (!OnRampWidgetConfig->GetDefaultFiatAmount().IsEmpty())
	{
		QueryParams.Add(FString(TEXT("defaultFiatAmount=")) + FPlatformHttp::UrlEncode(OnRampWidgetConfig->GetDefaultFiatAmount()));
	}

	if (!OnRampWidgetConfig->GetDefaultFiatCurrency().IsEmpty())
	{
		QueryParams.Add(FString(TEXT("defaultFiatCurrency=")) + FPlatformHttp::UrlEncode(OnRampWidgetConfig->GetDefaultFiatCurrency()));
	}

	if (!OnRampWidgetConfig->GetDefaultPaymentMethod().IsEmpty())
	{
		QueryParams.Add(FString(TEXT("defaultPaymentMethod=")) + FPlatformHttp::UrlEncode(OnRampWidgetConfig->GetDefaultPaymentMethod()));
	}

	if (OnRampWidgetConfig->GetCryptoCurrencyList().Num() > 0)
	{
		QueryParams.Add(FString(TEXT("cryptoCurrencyList=")) + FPlatformHttp::UrlEncode(FString::Join(OnRampWidgetConfig->GetCryptoCurrencyList(), TEXT(","))));
	}

	if (OnRampWidgetConfig->GetDisablePaymentMethods().Num() > 0)
	{
		QueryParams.Add(FString(TEXT("disablePaymentMethods=")) + FPlatformHttp::UrlEncode(FString::Join(OnRampWidgetConfig->GetDisablePaymentMethods(), TEXT(","))));
	}

	Path += TEXT("?");
	Path += FString::Join(QueryParams, TEXT("&"));

	return Path;
}

void UOnRampWidget::HandleOnUrlChanged(const FText& Text)
{
	if (Text.EqualToCaseIgnored(FText::FromString(InitialURL)))
	{
		bIsReady = true;
		OnWhenReady.Broadcast();
	}
}

#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
void UOnRampWidget::HandleOnConsoleMessage(const FString& Message, const FString& Source, int32 Line, EWebBrowserConsoleLogSeverity Severity)
{
	UE_LOG(LogImmutableOnRampWidget, Log, TEXT("Web Browser console message: %s, Source: %s, Line: %d"), *Message, *Source, Line);
}

bool UOnRampWidget::HandleOnBeforePopup(FString URL, FString Frame)
{
	return false;
}
#endif

#undef LOCTEXT_NAMESPACE