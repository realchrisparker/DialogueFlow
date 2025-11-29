// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationGraphDialogueNode.h
// Description: Editor graph node representing a dialogue node with an NPC line
//              and multiple branching player-selectable choices.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "Graph/Nodes/ConversationGraphNode.h"
#include "ConversationGraphDialogueNode.generated.h"

class UDialogueFlowDialogueNode;

/**
 * UConversationGraphDialogueNode
 *
 * Editor representation of a Dialogue node.
 *
 * Responsibilities:
 * - Owns a UDialogueFlowDialogueNode for the runtime data.
 * - Generates one input pin and N output pins (one for each dialogue choice).
 * - Keeps pin labels synced with choice titles.
 * - Maintains LinkedOutputPinIndex in the runtime node.
 * - Updates pin layout when Choices[] is modified in details panel.
 */
UCLASS()
class DIALOGUEFLOWEDITOR_API UConversationGraphDialogueNode : public UConversationGraphNode
{
    GENERATED_BODY()

public:

    // Constructor
    UConversationGraphDialogueNode(const FObjectInitializer& ObjectInitializer);
    
    // Destructor
    virtual ~UConversationGraphDialogueNode() override;

    // UEdGraphNode interface
    virtual void AllocateDefaultPins() override;

    /**
     * Returns the runtime dialogue node associated with this graph node.
     *
     * This automatically creates a UDialogueFlowDialogueNode if one does not
     * already exist, ensuring that the details panel and Slate widgets always
     * have a valid runtime node to work with.
     */
    UDialogueFlowDialogueNode* GetDialogueNode() const;

    /**
     * Returns the UObject that should be edited when this graph node
     * is selected in the editor. For dialogue nodes we expose the
     * underlying UDialogueFlowDialogueNode so designers edit runtime
     * properties directly.
     */
    virtual UObject* GetPropertyObject() const override;

#if WITH_EDITOR

    virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
    virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    virtual FLinearColor GetNodeTitleColor() const override;
    virtual FText GetTooltipText() const override;
    virtual void ReconstructNode() override;

    /** Called when a property changes on the runtime dialogue node */
    void HandleRuntimePropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent);

#endif

protected:

#if WITH_EDITOR
    /*
     * Functions
    */

    /** Rebuilds all pins to match the Choices array. */
    void RebuildOutputPins();

    /** Updates LinkedOutputPinIndex for all choices. */
    void SyncChoicePinIndices();

    /** Renames output pins based on ChoiceTitle text. */
    void RenameOutputPinsFromChoices();

    /** Bind/unbind runtime node events */
    void BindToRuntimeNode();
    void UnbindFromRuntimeNode();

    /*
     * Properties
    */

    /** Keep track of binding so we don’t double-bind */
    bool bIsBoundToRuntimeNode = false;

#endif

    /**
     * Ensures that a runtime UDialogueFlowDialogueNode exists and returns it.
     *
     * If no runtime node has been created yet, this will allocate one as
     * a child of the graph node and cache it in the DialogueNode pointer.
     */
    UFUNCTION(BlueprintCallable, Category = "Dialogue", meta = (DisplayName = "Get Or Create Dialogue Runtime Node"))
    UDialogueFlowDialogueNode* GetOrCreateDialogueNode();
};
