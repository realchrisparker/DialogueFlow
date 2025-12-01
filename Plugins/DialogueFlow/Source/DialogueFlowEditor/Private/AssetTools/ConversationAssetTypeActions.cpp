// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationAssetTypeActions.cpp
// Description: Implementation of asset type actions for Conversation Asset.
// ============================================================================

#include <AssetTools/ConversationAssetTypeActions.h>
#include "Assets/ConversationAsset.h"
#include "Editor/ConversationEditorToolkit.h"

#include "Toolkits/IToolkitHost.h"


UClass* FConversationAssetTypeActions::GetSupportedClass() const
{
    return UConversationAsset::StaticClass();
}

void FConversationAssetTypeActions::OpenAssetEditor(
    const TArray<UObject*>& InObjects,
    TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
    for (UObject* Obj : InObjects)
    {
        UConversationAsset* Asset = Cast<UConversationAsset>(Obj);
        if (Asset == nullptr)
        {
            continue;
        }

        // Create a new editor toolkit instance
        TSharedRef<FConversationEditorToolkit> EditorToolkit =
            MakeShared<FConversationEditorToolkit>();

        // Initialize the custom editor
        EditorToolkit->InitConversationEditor(EToolkitMode::Standalone, EditWithinLevelEditor, Asset);
    }
}
