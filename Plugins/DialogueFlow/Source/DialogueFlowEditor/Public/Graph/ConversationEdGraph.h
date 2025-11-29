// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationEdGraph.h
// Description: Base graph class for Dialogue Flow editor graphs.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include <Graph/Nodes/ConversationGraphStartNode.h>
#include "ConversationEdGraph.generated.h"


class UConversationGraphDialogueNode;
class UConversationGraphEndNode;
class UDialogueFlowDialogueNode;


/**
 * Custom graph used by Dialogue Flow assets.
 * Holds a reference to the Start Node and manages graph initialization.
 */
UCLASS()
class DIALOGUEFLOWEDITOR_API UConversationEdGraph : public UEdGraph
{
    GENERATED_BODY()

public:

    UConversationEdGraph();

    /** The Start Node created automatically when the graph is constructed. */
    UPROPERTY()
    UConversationGraphStartNode* StartNode;

#if WITH_EDITOR
    virtual void PostLoad() override;
    virtual void PostEditUndo() override;
#endif
    
    /** Rebuilds internal node references after undo/redo or loading. */
    void RebuildGraph();

    /** Validates the graph structure, fixes invalid nodes or pins, and ensures schema rules are enforced. */
    void ValidateGraph();    

private:

    /** Ensures all Dialogue Node runtime data is valid. */
    void ValidateDialogueNodes();

    /** Removes invalid connections, enforces choice-one-output rule. */
    void FixDialogueConnections();

    /** Performs cycle detection and breaks illegal cycles. */
    void BreakCycles();

    /** Depth-first search helper for cycle detection. */
    bool HasPathToNode(UEdGraphNode* Start, UEdGraphNode* Target) const;
};
