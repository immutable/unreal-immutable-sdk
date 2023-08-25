// Copyright Epic Games, Inc. All Rights Reserved.

#include "ImmutableModule.h"

#include "Immutable/Misc/ImtblLogging.h"
#include "Interfaces/IPluginManager.h"


#define LOCTEXT_NAMESPACE "FImmutableModule"

void FImmutableModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

#if USING_BUNDLED_CEF
    if (!IPluginManager::Get().FindPlugin("WebBrowserWidget")->IsEnabled())
    {
        IMTBL_LOG("Enabling WebBrowserWidget plugin...")
        if (!IPluginManager::Get().MountExplicitlyLoadedPlugin("WebBrowserWidget"))
        {
            IMTBL_ERR("Could not mount WebBrowserWidget plugin!")
        }
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