﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ImtblBlui.generated.h"

/**
 * 
 */
UCLASS()
class IMMUTABLE_API UImtblBlui : public UObject
{
	GENERATED_BODY()

public:
	UImtblBlui();

	UFUNCTION()
	void OnScriptEvent(const FString& EventName, const FString& EventMessage);
	TWeakObjectPtr<class UImtblJSConnector> GetJSConnector() const;
	
	UFUNCTION()
	void OnLogEvent(const FString& LogText);
	void WorldTickStart(UWorld* World, ELevelTick LevelTick, float X);

	virtual void BeginDestroy() override;
	void Init();

private:

	UPROPERTY()
	UObject* BluEyePtr = nullptr;

	bool bLodedIndexJS = false;
	
	UPROPERTY()
	class UImtblJSConnector* JSConnector = nullptr;
	FDelegateHandle WorldTickHandle;

	void ExecuteJS(const FString& ScriptText) const;
	
#if USING_BLUI_CEF
	class UBluEye* GetBluEye() const;
#endif
	
};
