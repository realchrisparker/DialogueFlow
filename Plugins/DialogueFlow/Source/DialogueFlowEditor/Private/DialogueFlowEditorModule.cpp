#include "DialogueFlowEditorModule.h"
#include <AssetTools/ConversationAssetTypeActions.h>


void FDialogueFlowEditorModule::StartupModule()
{
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

    // Create category: Dialogue Flow
    DialogueFlowAssetCategory = AssetTools.RegisterAdvancedAssetCategory(
        FName("DialogueFlow"),
        NSLOCTEXT("DialogueFlow", "DialogueFlowCategory", "Dialogue Flow")
    );

    // Register Conversation Asset type actions
    ConversationAssetActions = MakeShared<FConversationAssetTypeActions>(DialogueFlowAssetCategory);
    AssetTools.RegisterAssetTypeActions(ConversationAssetActions.ToSharedRef());
}

void FDialogueFlowEditorModule::ShutdownModule()
{
    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
        if (ConversationAssetActions.IsValid())
        {
            AssetTools.UnregisterAssetTypeActions(ConversationAssetActions.ToSharedRef());
        }
    }
}

IMPLEMENT_MODULE(FDialogueFlowEditorModule, DialogueFlowEditor);
