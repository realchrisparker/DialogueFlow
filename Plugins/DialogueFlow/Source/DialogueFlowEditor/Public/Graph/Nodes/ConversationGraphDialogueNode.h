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
class UEdGraphPin;


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

    /**
     * Allocates this node's pins.
     *
     * Layout:
     * - One input pin named "In".
     * - N output pins, one for each choice on the runtime dialogue node.
     */
    virtual void AllocateDefaultPins() override;

    /**
     * Rebuilds this node's pins when the structure changes (e.g., choices added
     * or removed), and restores connections for dynamic output pins where
     * possible.
     */
    virtual void ReconstructNode() override;

    /**
     * Returns the node's title text for display in the graph.
     * Uses the runtime node's NodeTitle when available, otherwise a fallback
     * localized "Dialogue" label.
     */
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

    /**
     * Ensures the node is reconstructed and the owning graph is refreshed after
     * an undo/redo operation.
     */
    virtual void PostEditUndo() override;

    /**
     * Returns the runtime UDialogueFlowDialogueNode bound to this editor node.
     * This simply casts the base UConversationGraphNode::GetNodeData result.
     */
    UFUNCTION(meta = (DisplayName = "Get Dialogue Runtime Node"))
    UDialogueFlowDialogueNode* GetDialogueNode() const;

#if WITH_EDITOR
    void HandleRuntimeNodePropertyChanged();
    
    /** Internal delegate shim receiving the FPropertyChangedEvent from the runtime node. */
    void HandleRuntimeNodePropertyChanged_Internal(const struct FPropertyChangedEvent& Event);
#endif

protected:

    /**
     * Restores output pin connections using the previous pin array.
     *
     * This is used after ReconstructNode has rebuilt pins to attempt to relink
     * any existing connections for dynamic output pins by matching pin names.
     */
    void RestoreDynamicConnections(const TArray<UEdGraphPin*>& OldPins);

    /**
     * Ensures output pin names match the associated dialogue choices.
     *
     * Rules:
     * - If a choice has a non-empty ChoiceTitle, use that text as the pin name.
     * - Otherwise, fallback to "Choice_<Index>".
     */
    void SyncPinNamesToChoices();
};
