using UnrealBuildTool;

public class FQPlayer : ModuleRules
{
    public FQPlayer(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "FQGameCore", "EnhancedInput", "Niagara" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}