// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImtblBlueprintAsyncAction.generated.h"
#include "Kismet/BlueprintAsyncActionBase.h"

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
