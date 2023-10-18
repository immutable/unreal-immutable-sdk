
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "ImtblSDKResourceAssetFactory.generated.h"

/**
 *
 */
UCLASS()
class IMMUTABLEEDITOR_API UImtblSDKResourceAssetFactory : public UFactory {
  GENERATED_BODY()

public:
  UImtblSDKResourceAssetFactory();

  bool FactoryCanImport(const FString &Filename) override;

  UObject *FactoryCreateBinary(UClass *InClass, UObject *InParent, FName InName,
                               EObjectFlags Flags, UObject *Context,
                               const TCHAR *Type, const uint8 *&Buffer,
                               const uint8 *BufferEnd,
                               FFeedbackContext *Warn) override;
};
