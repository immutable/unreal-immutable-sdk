#pragma once

#include "Components/Widget.h"

#include "TransakWebBrowser.generated.h"

#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
enum class EWebBrowserConsoleLogSeverity;

class SWebBrowser;
#else
class SBluWebBrowser;
#endif

/**
 * 
 */
UCLASS()
class IMMUTABLE_API UTransakWebBrowser : public UWidget
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnWhenReady);

public:
	UFUNCTION(BlueprintPure)
	bool IsReady() const;

	UFUNCTION(BlueprintCallable)
	void Load(const FString& WalletAddress, const FString& Email, const FString& ProductsAvailed, const FString& ScreenTitle);

	FDelegateHandle CallAndRegister_OnWhenReady(FOnWhenReady::FDelegate Delegate);

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

protected:
	FString ComputePath(const FString& WalletAddress, const FString& Email, const FString& ProductsAvailed, const FString& ScreenTitle);

	void HandleOnUrlChanged(const FText& Text);
#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
	void HandleOnConsoleMessage(const FString& Message, const FString& Source, int32 Line, EWebBrowserConsoleLogSeverity Severity);
	bool HandleOnBeforePopup(FString URL, FString Frame);
#endif

protected:
	/** URL that the browser will initially navigate to. The URL should include the protocol, eg http:// */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString InitialURL = TEXT("about:blank");

#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
	TSharedPtr<class SWebBrowser> WebBrowserWidget;
#else
	TSharedPtr<SBluWebBrowser> WebBrowserWidget;
#endif
	FOnWhenReady OnWhenReady;

protected:
	bool bIsReady = false;
};