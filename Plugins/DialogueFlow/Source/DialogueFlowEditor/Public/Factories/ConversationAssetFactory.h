// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationAssetFactory.h
// Description: Factory class used to create a new Conversation Asset from the
//              Content Browser. Allows creating .conversation assets.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "ConversationAssetFactory.generated.h"

class UConversationAsset;

/**
 * UConversationAssetFactory
 *
 * Factory responsible for creating new Conversation Asset files.
 * Appears in the "Create Asset" context menu under Miscellaneous.
 */
UCLASS()
class DIALOGUEFLOWEDITOR_API UConversationAssetFactory : public UFactory
{
    GENERATED_BODY()

public:

    /** Constructor */
    UConversationAssetFactory();

    /** Creates a new instance of a Conversation Asset */
    virtual UObject* FactoryCreateNew(
        UClass* Class,
        UObject* InParent,
        FName Name,
        EObjectFlags Flags,
        UObject* Context,
        FFeedbackContext* Warn
    ) override;

    /** Allows asset creation from the content browser */
    virtual bool ShouldShowInNewMenu() const override { return true; }
};
