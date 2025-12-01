// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationGraphEndNode.h
// Description: Editor End node. One input pin, no outputs.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "Graph/Nodes/ConversationGraphNode.h"
#include "ConversationGraphEndNode.generated.h"

/**
 * End node for Dialogue Flow:
 * - Only one INPUT pin is allowed
 * - No outgoing connections
 */
UCLASS()
class DIALOGUEFLOWEDITOR_API UConversationGraphEndNode : public UConversationGraphNode
{
    GENERATED_BODY()

public:

    UConversationGraphEndNode(const FObjectInitializer& ObjectInitializer);

    /** Allocates a single input pin and removes output pin. */
    virtual void AllocateDefaultPins() override;

    virtual void PostEditUndo() override;

    // virtual TSharedRef<SGraphNode> CreateVisualWidget() override;
};
