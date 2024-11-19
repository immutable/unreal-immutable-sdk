#pragma once

#include "Modules/ModuleInterface.h"

IMMUTABLEPLUGINMANAGER_API DECLARE_LOG_CATEGORY_EXTERN(LogImtblPluginManager, Log, All);

class FImmutablePluginManagerModule : public IModuleInterface
{
public:
	static void EnablePlugin(const FString& PluginName);
	static void DisablePlugin(const FString& PluginName);

	static void TogglePlugin(const FString& PluginName, bool bEnabled);

	static bool UpdateImmutablePluginDescriptor_PluginDependency(const FString& PluginName, bool bEnabled);
	static void UpdatePluginManager(const FString& PluginName, bool bEnabled);

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};