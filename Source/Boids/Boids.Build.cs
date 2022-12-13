using UnrealBuildTool;

public class Boids : ModuleRules
{
    public Boids(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Slate",
            "SlateCore",
            "UMG",
            "Systems",
            "ReactiveUserInterfaceSystems",
            "SystemsUnitTests",
            "Shared",
        });

        // PrivateDependencyModuleNames.AddRange(new string[] {});
    }
}
