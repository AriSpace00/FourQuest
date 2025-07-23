using UnrealBuildTool;

public class FQSoul : ModuleRules
{
    public FQSoul(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "FQGameCore", "EnhancedInput", "FQUI", "UMG", "Niagara" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}