// Fill out your copyright notice in the Description page of Project Settings.

#include "ImtblBrowserUserWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ScaleBox.h"
#include "Components/ScaleBoxSlot.h"

#include "Immutable/Browser/ImmutableJSConnectorBrowserWidget.h"
#include "Immutable/ImmutableUtilities.h"
#include "Immutable/Misc/ImtblLogging.h"

TSharedRef<SWidget> UImtblBrowserUserWidget::RebuildWidget()
{
	// RebuildWidget is not called until the widget is first added to the
	// viewport.

	UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());

	if (!RootWidget)
	{
		RootWidget = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootWidget"));
		if (UCanvasPanelSlot* RootWidgetSlot = Cast<UCanvasPanelSlot>(RootWidget->Slot))
		{
			// Format root widget
		}
		WidgetTree->RootWidget = RootWidget;
	}

	if (RootWidget)
	{
		UScaleBox* ScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("ScaleBox"));
		RootWidget->AddChild(ScaleBox);
		if (ScaleBox)
		{
			W_Browser = WidgetTree->ConstructWidget<UImmutableJSConnectorBrowserWidget>(UImmutableJSConnectorBrowserWidget::StaticClass(), TEXT("GameBridgeWidget"));
			W_Browser->MulticastDelegate_OnLoadCompleted().AddWeakLambda(this, []()
			{
#if PLATFORM_ANDROID | PLATFORM_IOS
				FString IndexURL = "file:///immutable/index.html";

#if USING_BUNDLED_CEF
				if (WebBrowserWidget->GetUrl() == IndexURL)
				{
					JSConnector->SetMobileBridgeReady();
				}
				else
				{
					UE_LOG(LogImmutable, Error, TEXT("Immutable Browser Widget Url don't match: (loaded : %s, required: %s)"), *WebBrowserWidget->GetUrl(), *IndexURL);
				}
#endif
#endif
			});
			W_Browser->MulticastDelegate_OnBrowserCreated()->AddWeakLambda(this, [this]()
			{
				FString JavaScript;
				if (FImmutableUtilities::LoadGameBridge(JavaScript))
				{
					FString IndexHtml = FString("<!doctype html><html lang='en'><head><meta " "charset='utf-8'><title>GameSDK Bridge</title><script>") + JavaScript + FString("</script></head><body><h1>Bridge Running</h1></body></html>");
					W_Browser->LoadString(IndexHtml, TEXT("http://localhost/"));
				}
			});
			ScaleBox->AddChild(W_Browser);
			if (UCanvasPanelSlot* RootWidgetSlot = Cast<UCanvasPanelSlot>(ScaleBox->Slot))
			{
#if PLATFORM_ANDROID | PLATFORM_IOS
				// Android webview needs to be at least 1px to 1px big to work
				// but it can be off screen
				RootWidgetSlot->SetAnchors(FAnchors(0, 0, 0, 0));
				RootWidgetSlot->SetOffsets(FMargin(-1, -1, 1, 1));
#else
				RootWidgetSlot->SetAnchors(FAnchors(0, 0, 1, 1));
				RootWidgetSlot->SetOffsets(DefaultOffsets);
#endif
			}
			if (UScaleBoxSlot* ScaleBoxSlot = Cast<UScaleBoxSlot>(W_Browser->Slot))
			{
				ScaleBoxSlot->SetHorizontalAlignment(HAlign_Fill);
				ScaleBoxSlot->SetVerticalAlignment(VAlign_Fill);
			}
		}
	}

	return Super::RebuildWidget();
}

void UImtblBrowserUserWidget::BeginDestroy() { Super::BeginDestroy(); }

void UImtblBrowserUserWidget::RemoveFromParent()
{
#if !PLATFORM_ANDROID
	// This is all that is needed to persist the widget state outside throughout
	// level/world changes. Super::RemoveFromParent();

	if (UPanelWidget* CurrentParent = GetParent())
	{
		CurrentParent->RemoveChild(this);
	}
#endif
}

void UImtblBrowserUserWidget::OnWidgetRebuilt()
{
#if PLATFORM_ANDROID | PLATFORM_IOS
	// Mobile webview needs to be visible to work
#else
	SetVisibility(ESlateVisibility::Collapsed);
#endif
	Super::OnWidgetRebuilt();
}

UImmutableJSConnectorBrowserWidget* UImtblBrowserUserWidget::GetBrowser() const
{
	return W_Browser;
}

TWeakObjectPtr<class UImtblJSConnector> UImtblBrowserUserWidget::GetJSConnector() const
{
	if (!W_Browser)
	{
		IMTBL_WARN("JSConnector requested before Browser was initialized");
		return nullptr;
	}

	return W_Browser->GetJSConnector();
}