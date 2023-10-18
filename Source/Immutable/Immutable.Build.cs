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
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PrivateDependencyModuleNames.Add("Blu");
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

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });

            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "Immutable_UPL_Android.xml"));
        }

        if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });

            PrivateIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Private", "IOS") });

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
            PrivateIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "Private", "Mac") });

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