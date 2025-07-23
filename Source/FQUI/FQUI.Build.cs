using UnrealBuildTool;

public class FQUI : ModuleRules
{
    public FQUI(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "FQGameCore", "EnhancedInput", "UMG", "AdvancedWidgets" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}