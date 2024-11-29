using UnrealBuildTool;

/**
 * ImmutablePluginManager establishes the rules for this module.
 */
public class ImmutablePluginManager : ModuleRules
{
	public ImmutablePluginManager(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Projects"
		});
	}
}