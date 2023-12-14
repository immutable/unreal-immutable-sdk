// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "ImtblBlueprintAsyncAction.generated.h"

/**
 
 * base class for asynchronous actions
 */
UCLASS()
class IMMUTABLE_API UImtblBlueprintAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

protected:
	UObject* WorldContextObject;

	// Get subsystem
	class UImmutableSubsystem* GetSubsystem() const;
};
