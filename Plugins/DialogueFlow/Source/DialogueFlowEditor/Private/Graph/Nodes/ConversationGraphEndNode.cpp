// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationGraphEndNode.cpp
// Description: Editor graph node representing an End node.
// ============================================================================

#include <Graph/Nodes/ConversationGraphEndNode.h>
#include <Graph/Nodes/SConversationGraphEndNode.h>
#include "EdGraph/EdGraphPin.h"


#define LOCTEXT_NAMESPACE "ConversationGraphNodes"

void UConversationGraphEndNode::AllocateDefaultPins()
{
    // End node: Only an input pin
    CreatePin(EGPD_Input, TEXT("DialogueFlow"), PinInput);
}

FText UConversationGraphEndNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("EndConversationTitle", "End Conversation");
}

FLinearColor UConversationGraphEndNode::GetNodeTitleColor() const
{
    return FLinearColor(1.0f, 0.25f, 0.25f); // Red-ish for End
}

FText UConversationGraphEndNode::GetTooltipText() const
{
    return LOCTEXT("EndNodeTooltip", "Marks the end of the conversation.");
}

FSlateIcon UConversationGraphEndNode::GetIconAndTint(FLinearColor& OutColor) const
{
    OutColor = FLinearColor(1.0f, 0.25f, 0.25f);
    return FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.Stop_16x");
}

TSharedPtr<SGraphNode> UConversationGraphEndNode::CreateVisualWidget()
{
    return SNew(SConversationGraphEndNode, this);
}

#undef LOCTEXT_NAMESPACE
