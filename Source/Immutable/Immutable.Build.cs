// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using UnrealBuildTool;

public class Immutable : ModuleRules
{
	public Immutable(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		#if UE_5_1_OR_LATER
			IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		#endif

		PublicIncludePaths.AddRange(
			new string[]
			{
				// ... add public include paths required here ...
			}
		);


		PrivateIncludePaths.AddRange(
			new string[]
			{
				// ... add other private include paths required here ...
			}
		);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Json",
				"JsonUtilities",
				"UMG",
				"Projects", 
				// ... add private dependencies that you statically link with here ...
			}
		);
		
		#if UE_5_0_OR_LATER
			PublicDependencyModuleNames.Add("WebBrowserWidget");
			PrivateDependencyModuleNames.Add("WebBrowser");
			PublicDefinitions.Add("USING_BUNDLED_CEF=1");
			PublicDefinitions.Add("USING_BLUI_CEF=0");
		#else 
			PrivateDependencyModuleNames.Add("Blu");
			PublicDefinitions.Add("USING_BUNDLED_CEF=0");
			PublicDefinitions.Add("USING_BLUI_CEF=1");
		#endif

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
	}
}