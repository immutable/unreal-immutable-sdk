// Copyright Epic Games, Inc. All Rights Reserved.

#include "ImmutableEditorModule.h"

#define LOCTEXT_NAMESPACE "FImmutableEditorModule"

void FImmutableEditorModule::StartupModule() {
  // This code will execute after your module is loaded into memory; the exact
  // timing is specified in the .uplugin file per-module
}

void FImmutableEditorModule::ShutdownModule() {
  // This code will execute after your module is loaded into memory; the exact
  // timing is specified in the .uplugin file per-module This function may be
  // called during shutdown to clean up your module.  For modules that support
  // dynamic reloading, we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FImmutableEditorModule, ImmutableEditor)
