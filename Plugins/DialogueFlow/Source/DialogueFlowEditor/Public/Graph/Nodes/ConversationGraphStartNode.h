// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationGraphStartNode.h
// Description: Editor Start node (entry point). Provides a single output pin
//              and no input pins. Used by UConversationEdGraph to enforce a
//              guaranteed starting point.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "Graph/Nodes/ConversationGraphNode.h"
#include "ConversationGraphStartNode.generated.h"

/**
 * Editor Start Node (entry point into conversation).
 *
 * Rules:
 * - Only one Start Node may exist in a graph.
 * - Only an OUTPUT pin is allowed.
 * - Cannot receive input connections.
 */
UCLASS()
class DIALOGUEFLOWEDITOR_API UConversationGraphStartNode : public UConversationGraphNode
{
    GENERATED_BODY()

public:

    UConversationGraphStartNode(const FObjectInitializer& ObjectInitializer);

    /** Allocates a single output pin. Removes any input pins inherited from base. */
    virtual void AllocateDefaultPins() override;

    /** Rebuilds node safely during undo/redo. */
    virtual void PostEditUndo() override;

    // virtual TSharedRef<SGraphNode> CreateVisualWidget() override;
};
