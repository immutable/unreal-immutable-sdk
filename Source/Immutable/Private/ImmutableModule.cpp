// Copyright Epic Games, Inc. All Rights Reserved.

#include "ImmutableModule.h"

#include "Immutable/Misc/ImtblLogging.h"
#include "Interfaces/IPluginManager.h"
#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include <psapi.h>
#include "Windows/HideWindowsPlatformTypes.h"
#endif

#define LOCTEXT_NAMESPACE "FImmutableModule"

void FImmutableModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact
	// timing is specified in the .uplugin file per-module

#if USING_BLUI_CEF
  if (IPluginManager::Get().FindPlugin("WebBrowserWidget") != nullptr &&
      IPluginManager::Get().FindPlugin("WebBrowserWidget")->IsEnabled()) {
    IMTBL_ERR("Cannot enable both BLUI and WebBrowserWidget plugin at the same "
              "time as it crashes.  In Immutable.uplugin file, "
              "'Plugins->WebBrowserWidget->Enabled' to 'false' and ensure the "
              "WebBrowserWidget is disabled in your project to use BLUI.")
  }
#endif
}

void FImmutableModule::ShutdownModule()
{}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FImmutableModule, Immutable)
