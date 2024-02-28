// Fill out your copyright notice in the Description page of Project Settings.

#include "Immutable/Actions/ImtblBlueprintAsyncAction.h"

UImmutableSubsystem* UImtblBlueprintAsyncAction::GetSubsystem() const
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		return nullptr;
	}
	return WorldContextObject->GetWorld()->GetGameInstance()->GetSubsystem<UImmutableSubsystem>();
}
