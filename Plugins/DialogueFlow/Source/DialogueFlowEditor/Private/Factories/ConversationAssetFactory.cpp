// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationAssetFactory.cpp
// Description: Implementation for Conversation Asset Factory.
// ============================================================================

#include <Factories/ConversationAssetFactory.h>
#include <Assets/ConversationAsset.h>

/*
 * Constructor
 */
UConversationAssetFactory::UConversationAssetFactory()
{
    SupportedClass = UConversationAsset::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}

/*
 * Creates a new instance of a Conversation Asset
 */
UObject* UConversationAssetFactory::FactoryCreateNew(
    UClass* Class,
    UObject* InParent,
    FName Name,
    EObjectFlags Flags,
    UObject* Context,
    FFeedbackContext* Warn
)
{
    return NewObject<UConversationAsset>(InParent, Class, Name, Flags);
}
