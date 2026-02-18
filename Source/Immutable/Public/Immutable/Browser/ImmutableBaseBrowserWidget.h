#pragma once

#include "Components/Widget.h"

#if USING_BUNDLED_CEF
#include "IWebBrowserWindow.h"
#endif

#include "ImmutableBaseBrowserWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FImmutableBrowserConsoleMessageDynamicMulticastDelegate, const FString&, Message, const FString&, Source, int32, Line, int32, Severity);

/**
 * Abstract base class for browser widgets providing core browser functionality
 * Derived classes can override template methods to customize behavior
 */
UCLASS(Abstract)
class IMMUTABLE_API UImmutableBaseBrowserWidget : public UWidget
{
	GENERATED_BODY()

public:
	/** UVisual: @Interface @Begin */
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	/** UVisual: @Interface @End */

	/**
	 * Check if a page is currently loaded
	 * @return True if a page is loaded
	 */
	UFUNCTION(BlueprintPure)
	virtual bool IsPageLoaded() const;

	/**
	 * Gets the currently loaded URL.
	 *
	 * @return The URL, or empty string if no document is loaded.
	 */
	UFUNCTION(BlueprintPure)
	FString GetUrl() const;

	UFUNCTION(BlueprintCallable)
	void LoadURL(FString NewURL) const;

	UFUNCTION(BlueprintCallable)
	virtual void LoadString(FString Contents, FString DummyURL);

	FImmutableBrowserConsoleMessageDynamicMulticastDelegate* DynamicMulticastDelegate_OnConsoleMessage();
	FSimpleMulticastDelegate MulticastDelegate_OnLoadCompleted();
	FSimpleMulticastDelegate* MulticastDelegate_OnBrowserCreated();

protected:
	/** UWidget: @Interface @Begin */
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void OnWidgetRebuilt() override;
	/** UWidget: @Interface @End */

#if PLATFORM_ANDROID || PLATFORM_IOS
	virtual void HandleOnLoadCompleted();
#endif

#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1
	void HandleOnConsoleMessage(const FString& Message, const FString& Source, int32 Line, EWebBrowserConsoleLogSeverity Severity);
#endif

	virtual void HandleLoadCompleted();
	virtual void HandleConsoleMessage(const FString& Message, const FString& Source, int32 Line, int32 Severity);

	virtual void OnBrowserCreated();

protected:
	/** Called when console messages are logged */
	UPROPERTY(BlueprintAssignable, Category = "Browser|Events", Meta = (DisplayName = "On Console Message"))
	FImmutableBrowserConsoleMessageDynamicMulticastDelegate Internal_DynamicMulticastDelegate_OnConsoleMessage;

	FSimpleMulticastDelegate Internal_MulticastDelegate_OnLoadCompleted;
	FSimpleMulticastDelegate Internal_MulticastDelegate_OnBrowserCreated;

#if USING_BUNDLED_CEF
	/** The underlying Slate web browser widget */
	TSharedPtr<SWebBrowser> WebBrowserWidget;
#endif

	/** URL that the browser will initially navigate to */
	FString InitialURL = TEXT("about:blank");

	/** Whether the browser supports transparency */
	bool bSupportsTransparency = true;

	/** Whether to show initial loading throbber */
	bool bShowInitialThrobber = false;
};