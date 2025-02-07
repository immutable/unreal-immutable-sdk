#include "OnRamp/ImmutableMarketplaceOnRampWidget.h"

#include "PlatformHttp.h"

#include "ImmutableMarketplaceLinkFactory.h"

#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
#include "SWebBrowser.h"
#else
#include "UserInterface/BluWebBrowser.h"
#endif

#define LOCTEXT_NAMESPACE "OnRampWebBrowserWidget"

DEFINE_LOG_CATEGORY(LogImmutableOnRampWidget);

UImmutableMarketplaceOnRampWidget::UImmutableMarketplaceOnRampWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	QueryParams.DefaultCryptoCurrency = TEXT("IMX");
	QueryParams.DefaultFiatAmount = TEXT("50");
	QueryParams.DefaultFiatCurrency = TEXT("USD");
	QueryParams.CryptoCurrencyList = TEXT("imx,eth,usdc");
}

bool UImmutableMarketplaceOnRampWidget::IsReady() const
{
	return bIsReady;
}

void UImmutableMarketplaceOnRampWidget::Load(const FString& WalletAddress, const FString& Email)
{
	if (!WebBrowserWidget.IsValid())
	{
		return;
	}

	const FString OnRampLink = UImmutableMarketplaceLinkFactory::GenerateOnRampLink(Environment, Email, WalletAddress, QueryParams, ExtraQueryParams);

	CallAndRegister_OnWhenReady(UImmutableMarketplaceOnRampWidget::FOnWhenReady::FDelegate::CreateWeakLambda(this, [this, OnRampLink]()
	{
		WebBrowserWidget->LoadURL(OnRampLink);
		OnWhenReady.RemoveAll(this);
	}));
}

FDelegateHandle UImmutableMarketplaceOnRampWidget::CallAndRegister_OnWhenReady(FOnWhenReady::FDelegate Delegate)
{
	if (bIsReady)
	{
		Delegate.ExecuteIfBound();
	}

	return OnWhenReady.Add(Delegate);
}

TSharedRef<SWidget> UImmutableMarketplaceOnRampWidget::RebuildWidget()
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

void UImmutableMarketplaceOnRampWidget::HandleOnUrlChanged(const FText& Text)
{
	if (Text.EqualToCaseIgnored(FText::FromString(InitialURL)))
	{
		bIsReady = true;
		OnWhenReady.Broadcast();
		OnWhenReady.Clear();
	}
}

#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
void UImmutableMarketplaceOnRampWidget::HandleOnConsoleMessage(const FString& Message, const FString& Source, int32 Line, EWebBrowserConsoleLogSeverity Severity)
{
	UE_LOG(LogImmutableOnRampWidget, Log, TEXT("Web Browser console message: %s, Source: %s, Line: %d"), *Message, *Source, Line);
}

bool UImmutableMarketplaceOnRampWidget::HandleOnBeforePopup(FString URL, FString Frame)
{
	return false;
}
#endif

#undef LOCTEXT_NAMESPACE