using UnrealBuildTool;

public class Tutorial : ModuleRules
{
    public Tutorial(ReadOnlyTargetRules Target) : base(Target)
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
            "Systems",
            "Shared",
        });

        // PrivateDependencyModuleNames.AddRange(new string[] {});
    }
}
