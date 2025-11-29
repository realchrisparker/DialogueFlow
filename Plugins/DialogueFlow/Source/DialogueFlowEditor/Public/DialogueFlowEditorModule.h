#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "AssetTypeCategories.h"


/**
 * DialogueFlowEditor Module
 * Registers asset actions and editor features for Dialogue assets.
 */
class FDialogueFlowEditorModule : public IModuleInterface
{
public:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:

    /** Category for Dialogue Flow assets */
    EAssetTypeCategories::Type DialogueFlowAssetCategory;

    // We store this to unregister it later
    TSharedPtr<class FConversationAssetTypeActions> ConversationAssetActions;
};
