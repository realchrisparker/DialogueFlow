// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationGraphNode.h
// Description: Base editor graph node with full pin connection serialization.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "ConversationGraphNode.generated.h"

/**
 * Saved connection data for one pin→pin relationship.
 */
USTRUCT()
struct FConversationSavedPinLink
{
    GENERATED_BODY()

    /** GUID of this pin */
    UPROPERTY()
    FGuid ThisPinId;

    /** GUID of the linked pin */
    UPROPERTY()
    FGuid LinkedPinId;
};

/**
 * Wrapper: all saved links for this node.
 */
USTRUCT()
struct FConversationNodeSavedConnection
{
    GENERATED_BODY()

    /** All saved connections */
    UPROPERTY()
    TArray<FConversationSavedPinLink> SavedLinks;
};

/**
 * Base class for all conversation graph nodes in the editor graph.
 */
UCLASS()
class DIALOGUEFLOWEDITOR_API UConversationGraphNode : public UEdGraphNode
{
    GENERATED_BODY()

public:

    UConversationGraphNode(const FObjectInitializer& ObjectInitializer);

    // Node type for identifying Start, End, Dialogue subclasses
    UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
    FName NodeClassName;

    // Saved connection data (per-node, serialized properly)
    UPROPERTY()
    FConversationNodeSavedConnection SavedConnectionData;

    // Called when pins need to be rebuilt
    virtual void AllocateDefaultPins() override;

    // Reconstruction (called by UE during load / undo / etc.)
    virtual void ReconstructNode() override;

    // Serialization
    virtual void Serialize(FArchive& Ar) override;

    // Called after load
    virtual void PostLoad() override;

    virtual void PostEditUndo() override;


    UFUNCTION()
    UDialogueFlowBaseNode* GetNodeData() const;

protected:

    /** Restores connections using GUIDs saved in SavedConnectionData */
    void RestoreConnections();
};
