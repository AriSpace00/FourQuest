using UnrealBuildTool;

public class FQQuest : ModuleRules
{
    public FQQuest(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "FQGameCore", "FQUI", "UMG", "AdvancedWidgets" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}