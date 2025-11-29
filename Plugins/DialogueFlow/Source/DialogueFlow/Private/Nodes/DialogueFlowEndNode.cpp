// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: DialogueFlowEndNode.cpp
// Description: Dialogue flow node that marks the end of a conversation.
// ============================================================================

#include <Nodes/DialogueFlowEndNode.h>
// #include "DialogueFlowComponent.h" 

/*
 * Constructor
*/
UDialogueFlowEndNode::UDialogueFlowEndNode()
{
    NodeDisplayName = FText::FromString("End");
    NodeTitle = FText::FromString("End Node");
    NodeColor = FLinearColor(0.85f, 0.15f, 0.15f); // Red-ish

    // End nodes never have outputs.
    OutputLinks.Empty();
}

void UDialogueFlowEndNode::OnExecuteNode(UDialogueFlowComponent* RuntimeComponent)
{
    // TODO: Implement when DialogueFlowComponent is created.
    ExecuteNext(RuntimeComponent);
}

#if WITH_EDITOR
void UDialogueFlowEndNode::ValidateNode(FDialogueFlowValidationContext& Context) const
{
    // Validation system not implemented yet.
}
#endif
