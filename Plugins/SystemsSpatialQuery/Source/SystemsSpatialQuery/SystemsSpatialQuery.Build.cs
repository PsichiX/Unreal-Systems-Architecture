using UnrealBuildTool;

public class SystemsSpatialQuery : ModuleRules
{
	public SystemsSpatialQuery(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        // NOTE: uncomment to ensure code conform to IWYU rules.
        // PCHUsage = ModuleRules.PCHUsageMode.NoPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
            }
        );


        PrivateIncludePaths.AddRange(
            new string[] {
            }
        );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
				"Systems",
            }
        );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
            }
        );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
        );
	}
}
