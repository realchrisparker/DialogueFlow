using UnrealBuildTool;

public class DialogueFlowEditor : ModuleRules
{
    public DialogueFlowEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[]
            {
                "DialogueFlowEditor/Public"
            }
        );

        PrivateIncludePaths.AddRange(
            new string[]
            {
                "DialogueFlowEditor/Private"
            }
        );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "EditorFramework",
                "Kismet",
                "AssetTools",
                "Projects",
                "PropertyEditor",
                "GraphEditor",
                "ApplicationCore"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "DialogueFlow",
                "ToolMenus",
                "Slate",
                "SlateCore",
                "UnrealEd"
            }
        );
    }
}
