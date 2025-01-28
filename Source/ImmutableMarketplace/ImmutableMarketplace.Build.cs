using UnrealBuildTool;

public class ImmutableMarketplace : ModuleRules
{
	public ImmutableMarketplace(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"DeveloperSettings",
			"HTTP"
		});

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"ImmutableCore"
		});

#if UE_5_1_OR_LATER
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"WebBrowser",
		});
#else
		PrivateDependencyModuleNames.Add("BluExtension");
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicDependencyModuleNames.Add("Blu");
		}
#endif
	}
}