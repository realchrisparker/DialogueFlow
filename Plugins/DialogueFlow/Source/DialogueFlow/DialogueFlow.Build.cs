using UnrealBuildTool;

public class DialogueFlow : ModuleRules
{
    public DialogueFlow(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "CoreUObject", "Engine" }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[] { }
        );
    }
}
