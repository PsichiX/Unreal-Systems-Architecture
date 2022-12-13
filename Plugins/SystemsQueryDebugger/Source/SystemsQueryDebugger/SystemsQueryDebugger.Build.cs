using UnrealBuildTool;

public class SystemsQueryDebugger : ModuleRules
{
    public SystemsQueryDebugger(ReadOnlyTargetRules Target) : base(Target)
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
                "Projects",
                "InputCore",
                "UnrealEd",
                "Blutility",
                "UMG",
                "UMGEditor",
                "ToolMenus",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "AssetRegistry",
            }
        );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
        );
    }
}
