// Fill out your copyright notice in the Description page of Project Settings.

#include "ImtblBrowserUserWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"
#include "Components/ScaleBox.h"
#include "Components/ScaleBoxSlot.h"
#include "Immutable/Misc/ImtblLogging.h"
#include "ImtblBrowserWidget.h"
#include "ImtblJSConnector.h"

TSharedRef<SWidget> UImtblBrowserUserWidget::RebuildWidget() {
  // RebuildWidget is not called until the widget is first added to the
  // viewport.

  UPanelWidget *RootWidget = Cast<UPanelWidget>(GetRootWidget());

  if (!RootWidget) {
    RootWidget = WidgetTree->ConstructWidget<UCanvasPanel>(
        UCanvasPanel::StaticClass(), TEXT("RootWidget"));
    if (UCanvasPanelSlot *RootWidgetSlot =
            Cast<UCanvasPanelSlot>(RootWidget->Slot)) {
      // Format root widget
    }
    WidgetTree->RootWidget = RootWidget;
  }

  if (RootWidget) {
    UScaleBox *ScaleBox = WidgetTree->ConstructWidget<UScaleBox>(
        UScaleBox::StaticClass(), TEXT("ScaleBox"));
    RootWidget->AddChild(ScaleBox);
    if (ScaleBox) {
      Browser = WidgetTree->ConstructWidget<UImtblBrowserWidget>(
          UImtblBrowserWidget::StaticClass(), TEXT("ImmutableBrowserWidget"));
      ScaleBox->AddChild(Browser);
      if (UCanvasPanelSlot *RootWidgetSlot =
              Cast<UCanvasPanelSlot>(ScaleBox->Slot)) {
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
      if (UScaleBoxSlot *ScaleBoxSlot = Cast<UScaleBoxSlot>(Browser->Slot)) {
        ScaleBoxSlot->SetHorizontalAlignment(HAlign_Fill);
        ScaleBoxSlot->SetVerticalAlignment(VAlign_Fill);
      }
    }
  }

  return Super::RebuildWidget();
}

void UImtblBrowserUserWidget::BeginDestroy() { Super::BeginDestroy(); }

void UImtblBrowserUserWidget::RemoveFromParent() {
#if !PLATFORM_ANDROID
  // This is all that is needed to persist the widget state outside throughout
  // level/world changes. Super::RemoveFromParent();

  if (UPanelWidget *CurrentParent = GetParent()) {
    CurrentParent->RemoveChild(this);
  }
#endif
}

void UImtblBrowserUserWidget::OnWidgetRebuilt() {
#if PLATFORM_ANDROID | PLATFORM_IOS
  // Mobile webview needs to be visible to work
#else
  SetVisibility(ESlateVisibility::Collapsed);
#endif
  Super::OnWidgetRebuilt();
}

TWeakObjectPtr<class UImtblJSConnector>
UImtblBrowserUserWidget::GetJSConnector() const {
  if (!Browser) {
    IMTBL_WARN("JSConnector requested before Browser was initialized");
    return nullptr;
  }

  return Browser->GetJSConnector();
}
