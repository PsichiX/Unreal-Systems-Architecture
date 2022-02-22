using UnrealBuildTool;

public class Shared : ModuleRules
{
    public Shared(ReadOnlyTargetRules Target) : base(Target)
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
            "Systems",
        });

        // PrivateDependencyModuleNames.AddRange(new string[] {});
    }
}
