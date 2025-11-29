// Copyright © 2025 God's Studio
// All Rights Reserved.

/**
 * UDialogueFlowNode_Start
 * The entry point of every Dialogue Flow asset.
 * This node has no properties and must be the first node executed.
 * Only one Start Node is allowed per Dialogue asset.
 * It cannot have any incoming connections.
 */

#pragma once

#include "CoreMinimal.h"
#include <Nodes/DialogueFlowBaseNode.h>
#include "DialogueFlowStartNode.generated.h"

UCLASS()
class DIALOGUEFLOW_API UDialogueFlowStartNode : public UDialogueFlowBaseNode
{
    GENERATED_BODY()

public:

    UDialogueFlowStartNode();

    /** Returns the node type as Start. */
    virtual EDialogueFlowNodeType GetNodeType() const override { return EDialogueFlowNodeType::Start; }

    /**
     * Executes the Start Node.
     * This will broadcast OnConversationStart on the component, then forward execution.
     */
    virtual void OnExecuteNode(UDialogueFlowComponent* RuntimeComponent) override;

#if WITH_EDITOR
    /**
     * Validation: Start Nodes cannot have inputs, and each asset must have exactly one.
     */
    virtual void ValidateNode(class FDialogueFlowValidationContext& Context) const override;
#endif
};
