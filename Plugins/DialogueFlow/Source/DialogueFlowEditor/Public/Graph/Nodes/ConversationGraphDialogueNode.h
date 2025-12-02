// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationGraphDialogueNode.h
// Description: Editor Dialogue node. Supports dynamic output pins that
//              correspond to runtime dialogue choices.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "Graph/Nodes/ConversationGraphNode.h"
#include "ConversationGraphDialogueNode.generated.h"

class UDialogueFlowDialogueNode;

/**
 * Editor Dialogue Node
 *
 * Contains:
 * - One input pin ("In")
 * - One dynamic output pin per dialogue choice
 *
 * When choices are added/removed in the Details panel, pins update and
 * connections are restored if GUIDs match.
 */
UCLASS()
class DIALOGUEFLOWEDITOR_API UConversationGraphDialogueNode : public UConversationGraphNode
{
    GENERATED_BODY()

public:

    UConversationGraphDialogueNode(const FObjectInitializer& ObjectInitializer);

    /** Rebuilds pins based on runtime data-node's `Choices`. */
    virtual void AllocateDefaultPins() override;

    /** Fully reconstruct pins + restore dynamic connections & names. */
    virtual void ReconstructNode() override;

    /** Undo/redo safe reconstruction. */
    virtual void PostEditUndo() override;

    /** Returns the underlying runtime dialogue node. */
    UFUNCTION()
    UDialogueFlowDialogueNode* GetDialogueNode() const
    {
        return Cast<UDialogueFlowDialogueNode>(GetNodeData());
    }

protected:

    /** Restores GUID-matched links for dynamic pins. */
    void RestoreDynamicConnections(const TArray<UEdGraphPin*>& OldPins);

    /** Ensures pin names match choice text or fallback indexed names. */
    void SyncPinNamesToChoices();
};
