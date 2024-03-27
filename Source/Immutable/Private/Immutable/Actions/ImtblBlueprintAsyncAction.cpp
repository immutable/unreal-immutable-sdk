// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/Actions/ImtblBlueprintAsyncAction.h"
#include "Immutable/ImmutableSubsystem.h"


UImmutableSubsystem* UImtblBlueprintAsyncAction::GetSubsystem() const
{
	if (!SavedWorldContextObject || !SavedWorldContextObject->GetWorld())
	{
		return nullptr;
	}
	return SavedWorldContextObject->GetWorld()->GetGameInstance()->GetSubsystem<UImmutableSubsystem>();
}
