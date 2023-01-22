using UnrealBuildTool;

public class SystemsSpatialQueryEditor : ModuleRules
{
	public SystemsSpatialQueryEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		bUseUnity = false;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		// NOTE: uncomment to ensure code conform to IWYU rules.
		// PCHUsage = ModuleRules.PCHUsageMode.NoPCHs;

		PublicIncludePaths.AddRange(
			new string[]
			{
			}
		);


		PrivateIncludePaths.AddRange(
			new string[]
			{
			}
		);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"SystemsSpatialQuery",
				"Core",
				// "Systems",
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"GeometryCore",
			}
		);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
		);
	}
}