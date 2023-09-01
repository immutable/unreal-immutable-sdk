﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ImtblSDKResourceAssetFactory.h"

#include "Immutable/Assets/ImtblSDKResource.h"
#include "Immutable/Misc/ImtblLogging.h"
#include "Misc/FileHelper.h"


UImtblSDKResourceAssetFactory::UImtblSDKResourceAssetFactory()
{
    Formats.Add(FString(TEXT("html; Immutable SDK Resource")));
    SupportedClass = UImtblSDKResource::StaticClass();
    bCreateNew = false;
    bEditorImport = true;
}


bool UImtblSDKResourceAssetFactory::FactoryCanImport(const FString& Filename)
{
    return FPaths::GetCleanFilename(Filename) == TEXT("index.html");
}


UObject* UImtblSDKResourceAssetFactory::FactoryCreateBinary(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn)
{
    UImtblSDKResource* Resource = NewObject<UImtblSDKResource>(InParent, InClass, InName, Flags);

    if (CurrentFilename.IsEmpty() || !FPaths::FileExists(CurrentFilename))
    {
        IMTBL_LOG("Invalid input html file: %s", *CurrentFilename)
        return nullptr;
    }

    FFileHelper::LoadFileToString(Resource->Html, *CurrentFilename, FFileHelper::EHashOptions::EnableVerify, FILEREAD_NoFail);

    const FString CompiledIndexJSFile = FPaths::Combine(FPaths::GetPath(CurrentFilename), TEXT("index.js"));

    if (CompiledIndexJSFile.IsEmpty() || !FPaths::FileExists(CompiledIndexJSFile))
    {
        IMTBL_LOG("Invalid input js file: %s", *CompiledIndexJSFile)
        return nullptr;
    }
    
    FFileHelper::LoadFileToString(Resource->Js, *CompiledIndexJSFile, FFileHelper::EHashOptions::EnableVerify, FILEREAD_NoFail);
    
    return Resource;
}
