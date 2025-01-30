#pragma once

#include "Components/Widget.h"

#include "ImmutableEnums.h"
#include "OnRamp/ImmutableOnRampQueryParams.h"

#include "ImmutableMarketplaceOnRampWidget.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogImmutableOnRampWidget, Log, All);

#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
enum class EWebBrowserConsoleLogSeverity;

class SWebBrowser;
#else
class SBluWebBrowser;
#endif

/**
 * A custom web browser widget for Immutable On Ramp funds transactions.
 */
UCLASS()
class IMMUTABLEMARKETPLACE_API UImmutableMarketplaceOnRampWidget : public UWidget
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FOnWhenReady);

public:
	UImmutableMarketplaceOnRampWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/**
	 * Check if the web browser widget is ready to be loaded.
	 *
	 * @return True if the widget is ready, false otherwise.
	 */
	UFUNCTION(BlueprintPure)
	bool IsReady() const;

	/**
	 * Loads on ramp widget with provided user data.
	 *
	 * @param WalletAddress The wallet address to load.
	 * @param Email The email associated with the user.
	 */
	UFUNCTION(BlueprintCallable)
	void Load(const FString& WalletAddress, const FString& Email);

	FDelegateHandle CallAndRegister_OnWhenReady(FOnWhenReady::FDelegate Delegate);

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

protected:
	void HandleOnUrlChanged(const FText& Text);
#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
	void HandleOnConsoleMessage(const FString& Message, const FString& Source, int32 Line, EWebBrowserConsoleLogSeverity Severity);
	bool HandleOnBeforePopup(FString URL, FString Frame);
#endif

protected:
	/** URL that the browser will initially navigate to. The URL should include the protocol, eg http:// */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString InitialURL = TEXT("about:blank");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EImmutableEnvironment Environment = EImmutableEnvironment::Sandbox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FImmutableOnRampQueryParams QueryParams;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FString, FString> ExtraQueryParams;

#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1)
	TSharedPtr<class SWebBrowser> WebBrowserWidget;
#else
	TSharedPtr<SBluWebBrowser> WebBrowserWidget;
#endif
	FOnWhenReady OnWhenReady;

protected:
	bool bIsReady = false;
};