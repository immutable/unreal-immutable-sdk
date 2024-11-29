#include "ImmutablePluginManager.h"

#include "Interfaces/IPluginManager.h"
#include "Misc/CoreDelegates.h"
#include "Misc/MessageDialog.h"
#include "Runtime/Launch/Resources/Version.h"

#include "Runtime/Projects/Private/PluginManager.h"

DEFINE_LOG_CATEGORY(LogImtblPluginManager);

#define LOCTEXT_NAMESPACE "FImmutablePluginManagerModule"

/**
 * ImmutablePluginManagerInternal manages the enabling/disabling of BLUI and WebBrowserWidget.
 */
class FImmutablePluginManagerInternal
{
public:
	FImmutablePluginManagerInternal()
	{
#if ENGINE_MAJOR_VERSION <= 4
		FImmutablePluginManagerModule::EnablePlugin(TEXT("BLUI"));
		FImmutablePluginManagerModule::DisablePlugin(TEXT("WebBrowserWidget"));
#else
		FImmutablePluginManagerModule::DisablePlugin(TEXT("BLUI"));
		FImmutablePluginManagerModule::EnablePlugin(TEXT("WebBrowserWidget"));
#endif
	}
};

TOptional<FImmutablePluginManagerInternal> GImmutablePluginManagerInternal(InPlace);

void FImmutablePluginManagerModule::EnablePlugin(const FString& PluginName)
{
	TogglePlugin(PluginName, true);
}

void FImmutablePluginManagerModule::DisablePlugin(const FString& PluginName)
{
	TogglePlugin(PluginName, false);
}

void FImmutablePluginManagerModule::TogglePlugin(const FString& PluginName, bool bEnabled)
{
	UpdateImmutablePluginDescriptor_PluginDependency(PluginName, bEnabled);

	UpdatePluginManager(PluginName, bEnabled);
}

bool FImmutablePluginManagerModule::UpdateImmutablePluginDescriptor_PluginDependency(const FString& PluginName, bool bEnabled)
{
	bool bUpdateImmutablePluginDescriptor = false;

	IPluginManager& PluginManager = IPluginManager::Get();

	TSharedPtr<IPlugin> ImmutablePlugin = PluginManager.FindPlugin("Immutable");
	ensureAlways(ImmutablePlugin.IsValid());

	FPluginDescriptor& ImmutablePluginDescriptor = const_cast<FPluginDescriptor&>(ImmutablePlugin->GetDescriptor());

	FPluginReferenceDescriptor* PluginDependencyReferenceDescriptor = ImmutablePluginDescriptor.Plugins.FindByPredicate([PluginName](const FPluginReferenceDescriptor& Descriptor)
	{
		return Descriptor.Name == PluginName;
	});

	if (PluginDependencyReferenceDescriptor)
	{
		if (PluginDependencyReferenceDescriptor->bEnabled != bEnabled)
		{
			PluginDependencyReferenceDescriptor->bEnabled = bEnabled;

			bUpdateImmutablePluginDescriptor = true;
		}
	}
	else
	{
		TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(PluginName);

		if (ensureAlways(Plugin.IsValid()))
		{
			ImmutablePluginDescriptor.Plugins.Add(FPluginReferenceDescriptor(Plugin->GetName(), bEnabled));

			bUpdateImmutablePluginDescriptor = true;
		}
	}

	if (bUpdateImmutablePluginDescriptor)
	{
		FText FailReason;
		const bool bUpdateDescriptor = ImmutablePlugin->UpdateDescriptor(ImmutablePluginDescriptor, FailReason);

		ensureAlways(bUpdateDescriptor);
		ensureAlways(FailReason.IsEmpty());
	}

	return bUpdateImmutablePluginDescriptor;
}

void FImmutablePluginManagerModule::UpdatePluginManager(const FString& PluginName, bool bEnabled)
{
	IPluginManager& PluginManager = IPluginManager::Get();
	TSharedPtr<IPlugin> PluginInterface = PluginManager.FindPlugin(PluginName);

	if (ensureAlways(PluginInterface.IsValid()))
	{
		FPlugin* Plugin = reinterpret_cast<FPlugin*>(PluginInterface.Get());

		if (ensureAlways(Plugin))
		{
			if (Plugin->bEnabled != bEnabled)
			{
				Plugin->bEnabled = bEnabled;
			}
		}
	}
}

void FImmutablePluginManagerModule::StartupModule()
{
}

void FImmutablePluginManagerModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FImmutablePluginManagerModule, ImmutablePluginManager)