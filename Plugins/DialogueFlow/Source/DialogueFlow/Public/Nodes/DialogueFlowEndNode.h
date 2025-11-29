// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: DialogueFlowEndNode.h
// Description: Dialogue flow node that marks the end of a conversation.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "DialogueFlowBaseNode.h"
#include "DialogueFlowEndNode.generated.h"

/**
 * End Node
 *
 * This node represents the termination point of a dialogue flow.
 * When reached, the DialogueFlowComponent will stop evaluating,
 * fire an "OnConversationEnd" event, and allow gameplay to resume.
 */
UCLASS(BlueprintType, EditInlineNew)
class DIALOGUEFLOW_API UDialogueFlowEndNode : public UDialogueFlowBaseNode
{
    GENERATED_BODY()

public:
    UDialogueFlowEndNode();

    /** Returns the node’s category string for debugging/UI. */
    virtual FString GetNodeCategory() const override { return TEXT("End"); }

    /** Returns the node's type enum. */
    virtual EDialogueFlowNodeType GetNodeType() const override { return EDialogueFlowNodeType::End; }

    /** Runtime behavior when this node is reached. */
    virtual void OnExecuteNode(class UDialogueFlowComponent* RuntimeComponent) override;

#if WITH_EDITOR
    /** Color used for the node’s body in the editor. */
    virtual FLinearColor GetNodeBodyColor() const override { return NodeColor; }

    /** Text shown inside the editor node body. */
    virtual FText GetNodeDescription() const override { return NodeTitle; }

    /** Validates graph rules such as requiring exactly 1 input. */
    virtual void ValidateNode(class FDialogueFlowValidationContext& Context) const override;
#endif
};
