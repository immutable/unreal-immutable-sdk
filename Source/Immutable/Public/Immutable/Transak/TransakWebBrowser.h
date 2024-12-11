#pragma once

#include "CoreMinimal.h"
#include "WebBrowser.h"

#include "TransakWebBrowser.generated.h"

/**
 * 
 */
UCLASS()
class IMMUTABLE_API UTransakWebBrowser : public UWebBrowser
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
	void OnUrlChanged(const FText& Text);

protected:
	FOnWhenReady OnWhenReady;

private:
	bool bIsReady = false;
	
};
