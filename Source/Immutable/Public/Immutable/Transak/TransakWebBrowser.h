#pragma once

#include "Components/Widget.h"

#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
#include "SWebBrowser.h"
#endif

#include "TransakWebBrowser.generated.h"

/**
 * 
 */
UCLASS()
class IMMUTABLE_API UTransakWebBrowser : public UWidget
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnWhenReady);
	
	UFUNCTION(BlueprintCallable)
	void Load(const FString& WalletAddress, const FString& Email, const FString& ProductsAvailed, const FString& ScreenTitle);

	bool IsReady() const { return bIsReady; };

	FDelegateHandle CallAndRegister_OnWhenReady(FOnWhenReady::FDelegate Delegate);

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

private:
	FString ComputePath(const FString& WalletAddress, const FString& Email, const FString& ProductsAvailed, const FString& ScreenTitle);

#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
	void HandleOnConsoleMessage(const FString& Message, const FString& Source, int32 Line, EWebBrowserConsoleLogSeverity Severity);
	void HandleOnUrlChanged(const FText& Text);
	bool HandleOnBeforePopup(FString URL, FString Frame);
#endif

protected:
#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
	TSharedPtr<class SWebBrowser> WebBrowserWidget;
#else
	TSharedPtr<class SNullWidget> WebBrowserWidget;
#endif
	FOnWhenReady OnWhenReady;

private:
	bool bIsReady = false;
	
};
