#include "Immutable/Transak/TransakWebBrowser.h"

#include "PlatformHttp.h"
#include "Immutable/ImmutableUtilities.h"
#include "Immutable/TransakConfig.h"


#define LOCTEXT_NAMESPACE "Immutable"

FDelegateHandle UTransakWebBrowser::CallAndRegister_OnWhenReady(FOnWhenReady::FDelegate Delegate)
{
	if (bIsReady)
	{
		Delegate.ExecuteIfBound();
	}

	return OnWhenReady.Add(Delegate);
}

TSharedRef<SWidget> UTransakWebBrowser::RebuildWidget()
{
	if ( IsDesignTime() )
	{
		return SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("Transak Web Browser", "Transak Web Browser"))
			];
	}
	else
	{
#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
		WebBrowserWidget = SNew(SWebBrowser)
			.InitialURL(TEXT("about:blank"))
			.ShowControls(false)
			.SupportsTransparency(bSupportsTransparency)
			.OnUrlChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, OnUrlChanged))
			.OnBeforePopup(BIND_UOBJECT_DELEGATE(FOnBeforePopupDelegate, HandleOnBeforePopup))
			.OnConsoleMessage(BIND_UOBJECT_DELEGATE(FOnConsoleMessageDelegate, HandleOnConsoleMessage));
		return WebBrowserWidget.ToSharedRef();
#else
		return SNullWidget::NullWidget;
#endif
	}
}

void UTransakWebBrowser::OnUrlChanged(const FText& Text)
{
	if (Text.EqualToCaseIgnored(FText::FromString(TEXT("about:blank"))))
	{
		bIsReady = true;
		OnWhenReady.Broadcast();
	}
}

void UTransakWebBrowser::Load(const FString& WalletAddress, const FString& Email, const FString& ProductsAvailed, const FString& ScreenTitle)
{
    if (!WebBrowserWidget.IsValid())
    {
        return;
    }
    
    FString UrlToLoad = ComputePath(WalletAddress, Email, ProductsAvailed, ScreenTitle);

    if (bIsReady)
    {
#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
        WebBrowserWidget->LoadURL(UrlToLoad);
#endif
    }
    else
    {
        FDelegateHandle OnWhenReadyHandle = CallAndRegister_OnWhenReady(UTransakWebBrowser::FOnWhenReady::FDelegate::CreateWeakLambda(this, [this, UrlToLoad, OnWhenReadyHandle]()
        {
#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
            WebBrowserWidget->LoadURL(UrlToLoad);
#endif
        	OnWhenReady.Remove(OnWhenReadyHandle);
        }));
    }
}

FString UTransakWebBrowser::ComputePath(const FString& WalletAddress, const FString& Email, const FString& ProductsAvailed, const FString& ScreenTitle)
{
    UTransakConfig* TransakConfig = FImmutableUtilities::GetDefaultTransakConfig();

    if (!TransakConfig)
    {
        return "";
    }

    FString Path = TransakConfig->GetURL();
    TArray<FString> QueryParams;
    
    QueryParams.Add(FString(TEXT("apiKey=")) + FPlatformHttp::UrlEncode(TransakConfig->GetAPIKey()));
    QueryParams.Add(FString(TEXT("email=")) + FPlatformHttp::UrlEncode(Email));
    QueryParams.Add(FString(TEXT("walletAddress=")) + FPlatformHttp::UrlEncode(WalletAddress));
    QueryParams.Add(FString(TEXT("themeColor=")) + FPlatformHttp::UrlEncode(TransakConfig->GetThemeColor().ToString()));
    QueryParams.Add(FString(TEXT("isAutoFillUserData=")) + FPlatformHttp::UrlEncode(TransakConfig->IsAutoFillUserData() ? TEXT("true") : TEXT("false")));
    QueryParams.Add(FString(TEXT("disableWalletAddressForm=")) + FPlatformHttp::UrlEncode(TransakConfig->DisableWalletAddressForm() ? TEXT("true") : TEXT("false")));

    if (!TransakConfig->GetNetwork().IsEmpty())
    {
        QueryParams.Add(FString(TEXT("network=")) + FPlatformHttp::UrlEncode(TransakConfig->GetNetwork()));
    }
    
    if (!ProductsAvailed.IsEmpty())
    {
        QueryParams.Add(FString(TEXT("productsAvailed=")) + FPlatformHttp::UrlEncode(ProductsAvailed));    
    }
    
    if (!ScreenTitle.IsEmpty())
    {
        QueryParams.Add(FString(TEXT("exchangeScreenTitle=")) + FPlatformHttp::UrlEncode(ScreenTitle));    
    }
    
    if (!TransakConfig->GetDefaultCryptoCurrency().IsEmpty())
    {
        QueryParams.Add(FString(TEXT("defaultCryptoCurrency=")) + FPlatformHttp::UrlEncode(TransakConfig->GetDefaultCryptoCurrency()));
    }

    if (!TransakConfig->GetDefaultFiatAmount().IsEmpty())
    {
        QueryParams.Add(FString(TEXT("defaultFiatAmount=")) + FPlatformHttp::UrlEncode(TransakConfig->GetDefaultFiatAmount()));
    }
    
    if (!TransakConfig->GetDefaultFiatCurrency().IsEmpty())
    {
        QueryParams.Add(FString(TEXT("defaultFiatCurrency=")) + FPlatformHttp::UrlEncode(TransakConfig->GetDefaultFiatCurrency()));
    }

    if (!TransakConfig->GetDefaultPaymentMethod().IsEmpty())
    {
        QueryParams.Add(FString(TEXT("defaultPaymentMethod=")) + FPlatformHttp::UrlEncode(TransakConfig->GetDefaultPaymentMethod()));
    }

    if (TransakConfig->GetCryptoCurrencyList().Num() > 0)
    {
        QueryParams.Add(FString(TEXT("cryptoCurrencyList=")) + FPlatformHttp::UrlEncode(FString::Join(TransakConfig->GetCryptoCurrencyList(), TEXT(","))));
    }
    
    if (TransakConfig->GetDisablePaymentMethods().Num() > 0)
    {
        QueryParams.Add(FString(TEXT("disablePaymentMethods=")) + FPlatformHttp::UrlEncode(FString::Join(TransakConfig->GetDisablePaymentMethods(), TEXT(","))));
    }
    
    Path += TEXT("?");
    Path += FString::Join(QueryParams, TEXT("&"));

    return Path;
}
