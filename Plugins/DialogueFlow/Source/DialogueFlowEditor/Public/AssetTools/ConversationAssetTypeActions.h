// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationAssetTypeActions.h
// Description: Asset type actions for Conversation Asset. 
//              Handles asset category, icon, and editor opening behavior.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class UConversationAsset;

/**
 * FConversationAssetTypeActions
 *
 * Registers Conversation Asset with the editor.
 * Provides menu display name, category, color, and editor opening logic.
 */
class FConversationAssetTypeActions : public FAssetTypeActions_Base
{
public:
    FConversationAssetTypeActions(EAssetTypeCategories::Type InAssetCategory)
        : AssetCategory(InAssetCategory)
    {
    }

    virtual FText GetName() const override
    {
        return NSLOCTEXT("AssetTypeActions", "FConversationAsset", "Conversation");
    }

    virtual FColor GetTypeColor() const override
    {
        return FColor(175, 85, 255); // Purple-ish placeholder
    }

    virtual UClass* GetSupportedClass() const override;

    virtual uint32 GetCategories() override
    {
        return AssetCategory;
    }

    /** Opens our custom Conversation Editor */
    virtual void OpenAssetEditor(const TArray<UObject*>& InObjects,
        TSharedPtr<class IToolkitHost> EditWithinLevelEditor) override;

private:
    EAssetTypeCategories::Type AssetCategory;
};
