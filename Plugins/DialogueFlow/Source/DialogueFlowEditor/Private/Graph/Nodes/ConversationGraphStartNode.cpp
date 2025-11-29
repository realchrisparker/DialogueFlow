// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationGraphStartNode.cpp
// Description: Editor graph node representing a Start node.
// ============================================================================

#include <Graph/Nodes/ConversationGraphStartNode.h>
#include <Graph/Nodes/SConversationGraphStartNode.h>
#include "EdGraph/EdGraphPin.h"


#define LOCTEXT_NAMESPACE "ConversationGraphNodes"

void UConversationGraphStartNode::AllocateDefaultPins()
{
    // Start node: Only an output pin
    CreatePin(EGPD_Output, TEXT("DialogueFlow"), PinOutput);
}

FText UConversationGraphStartNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    return LOCTEXT("StartConversationTitle", "Start Conversation");
}

FLinearColor UConversationGraphStartNode::GetNodeTitleColor() const
{
    return FLinearColor(0.20f, 0.65f, 1.0f); // Blue-ish for Start
}

FText UConversationGraphStartNode::GetTooltipText() const
{
    return LOCTEXT("StartNodeTooltip", "Entry point for the conversation.");
}

FSlateIcon UConversationGraphStartNode::GetIconAndTint(FLinearColor& OutColor) const
{
    OutColor = FLinearColor(0.20f, 0.65f, 1.0f);
    return FSlateIcon(FAppStyle::GetAppStyleSetName(), "GraphEditor.Event_16x");
}

TSharedPtr<SGraphNode> UConversationGraphStartNode::CreateVisualWidget()
{
    return SNew(SConversationGraphStartNode, this);
}

#undef LOCTEXT_NAMESPACE