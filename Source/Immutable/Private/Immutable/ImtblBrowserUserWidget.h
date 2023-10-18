// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "ImtblBrowserUserWidget.generated.h"

UCLASS()
class IMMUTABLE_API UImtblBrowserUserWidget : public UUserWidget {
  GENERATED_BODY()
public:
  TSharedRef<SWidget> RebuildWidget() override;
  void BeginDestroy() override;
  void RemoveFromParent() override;
  void OnWidgetRebuilt() override;

  TWeakObjectPtr<class UImtblJSConnector> GetJSConnector() const;

private:
  UPROPERTY()
  class UImtblBrowserWidget *Browser = nullptr;

  bool bIsBrowserAppInitialized = false;

  FTimerHandle Timer;

  FMargin DefaultOffsets = FMargin(150, 150, 150, 150);
};
