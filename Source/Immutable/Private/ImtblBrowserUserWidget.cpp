// Fill out your copyright notice in the Description page of Project Settings.


#include "ImtblBrowserUserWidget.h"

#include "ImtblBrowserWidget.h"
#include "ImtblJSConnector.h"
#include "ImtblLogging.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"
#include "Components/ScaleBox.h"
#include "Components/ScaleBoxSlot.h"



TSharedRef<SWidget> UImtblBrowserUserWidget::RebuildWidget()
{
    // RebuildWidget is not called until the widget is first added to the viewport.

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
			Browser = WidgetTree->ConstructWidget<UImtblBrowserWidget>(UImtblBrowserWidget::StaticClass(), TEXT("ImmutableBrowserWidget"));
			ScaleBox->AddChild(Browser);
            if (UCanvasPanelSlot* RootWidgetSlot = Cast<UCanvasPanelSlot>(ScaleBox->Slot))
			{
				RootWidgetSlot->SetAnchors(FAnchors(0, 0, 1, 1));
				RootWidgetSlot->SetOffsets(DefaultOffsets);
			}
            if (UScaleBoxSlot* ScaleBoxSlot = Cast<UScaleBoxSlot>(Browser->Slot))
			{
				ScaleBoxSlot->SetHorizontalAlignment(HAlign_Fill);
				ScaleBoxSlot->SetVerticalAlignment(VAlign_Fill);
			}
		}
	}

    return Super::RebuildWidget();
}


void UImtblBrowserUserWidget::BeginDestroy()
{
	Super::BeginDestroy();
}


void UImtblBrowserUserWidget::RemoveFromParent()
{
    // This is all that is needed to persist the widget state outside throughout level/world changes.
	// Super::RemoveFromParent();

	if (UPanelWidget* CurrentParent = GetParent())
	{
		CurrentParent->RemoveChild(this);
	}
}


void UImtblBrowserUserWidget::OnWidgetRebuilt()
{
    SetVisibility(ESlateVisibility::Collapsed);
    Super::OnWidgetRebuilt();
}


TWeakObjectPtr<class UImtblJSConnector> UImtblBrowserUserWidget::GetJSConnector() const
{
    if (!Browser)
    {
        IMTBL_WARN("JSConnector requested before Browser was initialized");
        return nullptr;
    }

    return Browser->GetJSConnector();
}



