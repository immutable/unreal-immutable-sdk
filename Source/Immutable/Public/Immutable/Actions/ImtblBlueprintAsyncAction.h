// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
// clang-format off
#include "ImtblBlueprintAsyncAction.generated.h"
// clang-format on

/**
 *
 */
UCLASS()
class IMMUTABLE_API UImtblBlueprintAsyncAction
    : public UBlueprintAsyncActionBase {
  GENERATED_BODY()

protected:
  UObject *WorldContextObject;

  // Get subsystem
  class UImmutableSubsystem *GetSubsystem() const;
};
