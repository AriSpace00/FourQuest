using UnrealBuildTool;

public class FQMonster: ModuleRules
{
    public FQMonster(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "FQGameCore", "NavigationSystem", "Niagara" });
    }
}
