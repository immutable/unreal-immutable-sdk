#include "Immutable/Browser/ImmutableBaseBrowserWidget.h"

#include "Immutable/Misc/ImtblLogging.h"

#define LOCTEXT_NAMESPACE "BaseBrowserWidget"

void UImmutableBaseBrowserWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

#if USING_BUNDLED_CEF
	WebBrowserWidget.Reset();
#endif
}

bool UImmutableBaseBrowserWidget::IsPageLoaded() const
{
#if USING_BUNDLED_CEF
	return WebBrowserWidget.IsValid() && WebBrowserWidget->IsLoaded();
#endif
	return false;
}

FString UImmutableBaseBrowserWidget::GetUrl() const
{
#if USING_BUNDLED_CEF
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->GetUrl();
	}
#endif
	return FString();
}

void UImmutableBaseBrowserWidget::LoadURL(FString NewURL) const
{
#if USING_BUNDLED_CEF
	if (WebBrowserWidget.IsValid())
	{
		return WebBrowserWidget->LoadURL(NewURL);
	}
#endif
}

void UImmutableBaseBrowserWidget::LoadString(FString Contents, FString DummyURL)
{
#if USING_BUNDLED_CEF
	if (WebBrowserWidget.IsValid())
	{
		WebBrowserWidget->LoadString(Contents, DummyURL);
	}
#endif
}

FImmutableBrowserConsoleMessageDynamicMulticastDelegate* UImmutableBaseBrowserWidget::DynamicMulticastDelegate_OnConsoleMessage()
{
	return &Internal_DynamicMulticastDelegate_OnConsoleMessage;
}

FSimpleMulticastDelegate* UImmutableBaseBrowserWidget::MulticastDelegate_OnLoadCompleted()
{
	return &Internal_MulticastDelegate_OnLoadCompleted;
}

FSimpleMulticastDelegate* UImmutableBaseBrowserWidget::MulticastDelegate_OnBrowserCreated()
{
	return &Internal_MulticastDelegate_OnBrowserCreated;
}

TSharedRef<SWidget> UImmutableBaseBrowserWidget::RebuildWidget()
{
	if (IsDesignTime())
	{
		// Show placeholder in editor
		return SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("BrowserPlaceholder", "Browser Widget"))
			];
	}
	else
	{
#if USING_BUNDLED_CEF
		// Create the web browser widget
		WebBrowserWidget = SNew(SWebBrowser)
				.InitialURL(InitialURL)
				.ShowControls(false)
				.SupportsTransparency(bSupportsTransparency)
				.ShowInitialThrobber(bShowInitialThrobber)
#if PLATFORM_ANDROID || PLATFORM_IOS
			.OnLoadCompleted(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleOnLoadCompleted))
#endif
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
				.OnConsoleMessage(BIND_UOBJECT_DELEGATE(FOnConsoleMessageDelegate, HandleOnConsoleMessage))
#endif
			;

		return WebBrowserWidget.ToSharedRef();
#else
		// Fallback for non-CEF builds
		return
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("NoCEF", "Browser Not Available"))
			];
#endif
	}
}

void UImmutableBaseBrowserWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	OnBrowserCreated();
}

#if PLATFORM_ANDROID || PLATFORM_IOS
void UImmutableBaseBrowserWidget::HandleOnLoadCompleted()
{
	// Default mobile load handling
	HandleLoadCompleted();
}
#endif

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
void UImmutableBaseBrowserWidget::HandleOnConsoleMessage(const FString& Message, const FString& Source, int32 Line, EWebBrowserConsoleLogSeverity Severity)
{
	UE_LOG(LogImmutable, Log, TEXT("Browser console message: %s, Source: %s, Line: %d, Severity: %d"), *Message, *Source, Line, Severity);
	HandleConsoleMessage(Message, Source, Line, static_cast<int32>(Severity));
}
#endif

void UImmutableBaseBrowserWidget::HandleLoadCompleted()
{
	Internal_MulticastDelegate_OnLoadCompleted.Broadcast();
}

void UImmutableBaseBrowserWidget::HandleConsoleMessage(const FString& Message, const FString& Source, int32 Line, int32 Severity)
{
	Internal_DynamicMulticastDelegate_OnConsoleMessage.Broadcast(Message, Source, Line, Severity);
}

void UImmutableBaseBrowserWidget::OnBrowserCreated()
{
	Internal_MulticastDelegate_OnBrowserCreated.Broadcast();
}

#undef LOCTEXT_NAMESPACE