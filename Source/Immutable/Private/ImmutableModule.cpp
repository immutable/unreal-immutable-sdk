// Copyright Epic Games, Inc. All Rights Reserved.

#include "ImmutableModule.h"

#include "Immutable/Misc/ImtblLogging.h"
#include "Interfaces/IPluginManager.h"


#define LOCTEXT_NAMESPACE "FImmutableModule"

void FImmutableModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

#if USING_BLUI_CEF
    if (IPluginManager::Get().FindPlugin("WebBrowserWidget")->IsEnabled())
    {
        IMTBL_WARN("Can not enable both Blui and WebBrowserWidget plugin at the same time as it crashes. In Immutable.uplugin file, 'Plugins->WebBrowserWidget->Enabled' to false to use Blui")
    }
#endif
}


void FImmutableModule::ShutdownModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE


IMPLEMENT_MODULE(FImmutableModule, Immutable)