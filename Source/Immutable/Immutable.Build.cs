// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class Immutable : ModuleRules
{
	public Immutable(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

#if UE_5_1_OR_LATER
			IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
#endif
		RuntimeDependencies.Add("$(PluginDir)/Web/index.js");

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"JsonUtilities"
		});

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"ImmutableCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"Json",
			"UMG",
			"Projects",
			"DeveloperSettings",
			"HTTP",
		});

#if UE_5_1_OR_LATER
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"WebBrowser",
				"WebBrowserWidget"
			}
		);
#else
		PrivateDependencyModuleNames.Add("BluExtension");
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicDependencyModuleNames.Add("Blu");
		}
#endif

#if UE_5_1_OR_LATER
			PrivateDependencyModuleNames.Add("WebBrowser");
			PublicDefinitions.Add("USING_BUNDLED_CEF=1");
			PublicDefinitions.Add("USING_BLUI_CEF=0");
#else
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicDefinitions.Add("USING_BLUI_CEF=1");
		}
		else
		{
			PublicDefinitions.Add("USING_BLUI_CEF=0");
		}

		PublicDefinitions.Add("USING_BUNDLED_CEF=0");
#endif

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);

		if (Target.bBuildEditor == true)
		{
			PrivateDependencyModuleNames.Add("UnrealEd");
		}

		if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });

			string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
			AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "Immutable_UPL_Android.xml"));
		}

		if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });

			PrivateIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Private/Immutable", "IOS") });

			PublicFrameworks.AddRange(
				new string[]
				{
					"Foundation",
					"AuthenticationServices"
				}
			);
		}

		if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			PrivateIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Private/Immutable", "Mac") });

			PublicFrameworks.AddRange(
				new string[]
				{
					"Foundation",
					"AuthenticationServices"
				}
			);
		}
	}
}