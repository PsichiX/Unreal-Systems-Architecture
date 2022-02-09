using UnrealBuildTool;

public class Systems_Workspace : ModuleRules
{
    public Systems_Workspace(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Slate",
            "SlateCore",
            "UMG",
            "Paper2D",
            "NavigationSystem",
            "AiModule"
        });

        // PrivateDependencyModuleNames.AddRange(new string[] {});
    }
}
