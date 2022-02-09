using UnrealBuildTool;
using System.Collections.Generic;

public class Systems_WorkspaceEditorTarget : TargetRules
{
    public Systems_WorkspaceEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("Systems_Workspace");
    }
}
