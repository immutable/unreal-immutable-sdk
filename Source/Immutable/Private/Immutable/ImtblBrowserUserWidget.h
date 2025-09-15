#pragma once

#include "Blueprint/UserWidget.h"

#include "ImtblBrowserUserWidget.generated.h"

class UImmutableJSConnectorBrowserWidget;

UCLASS()
class IMMUTABLE_API UImtblBrowserUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void BeginDestroy() override;
	virtual void RemoveFromParent() override;
	virtual void OnWidgetRebuilt() override;

	UImmutableJSConnectorBrowserWidget* GetBrowser() const;
	TWeakObjectPtr<class UImtblJSConnector> GetJSConnector() const;

private:
	UPROPERTY()
	TObjectPtr<UImmutableJSConnectorBrowserWidget> W_Browser;

	bool bIsBrowserAppInitialized = false;

	FTimerHandle Timer;

	FMargin DefaultOffsets = FMargin(150, 150, 150, 150);
};