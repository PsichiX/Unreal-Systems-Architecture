using UnrealBuildTool;
using System.Collections.Generic;

public class Systems_WorkspaceTarget : TargetRules
{
    public Systems_WorkspaceTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        ExtraModuleNames.Add("Boids");
        ExtraModuleNames.Add("Life");
        ExtraModuleNames.Add("Multiplayer");
        ExtraModuleNames.Add("Tutorial");
        ExtraModuleNames.Add("Shared");
    }
}
