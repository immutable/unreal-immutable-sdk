// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"

#include "ImmutableSaveGame.generated.h"

/**
 * Immutable sdk save game object
 */
UCLASS(BlueprintType, Blueprintable)
class IMMUTABLE_API UImmutableSaveGame : public USaveGame
{
	GENERATED_UCLASS_BODY()

public:

	/** check if player logged in/connected with PKCE flow previously */
	UPROPERTY(VisibleAnywhere, Category = "Immutable")
	bool bWasConnectedViaPKCEFlow;

};