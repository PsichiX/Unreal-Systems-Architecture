using UnrealBuildTool;

public class Multiplayer : ModuleRules
{
    public Multiplayer(ReadOnlyTargetRules Target) : base(Target)
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
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
            "Systems",
            "Shared",
        });

        // PrivateDependencyModuleNames.AddRange(new string[] {});
    }
}
