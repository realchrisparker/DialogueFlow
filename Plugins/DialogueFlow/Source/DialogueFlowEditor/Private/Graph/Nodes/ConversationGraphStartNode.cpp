// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationGraphStartNode.cpp
// Description: Implementation of the Start Node for Dialogue Flow.
// ============================================================================

#include "Graph/Nodes/ConversationGraphStartNode.h"
#include "EdGraph/EdGraphPin.h"

UConversationGraphStartNode::UConversationGraphStartNode(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    NodeClassName = "Start";
}

void UConversationGraphStartNode::AllocateDefaultPins()
{
    // Base class creates In + Out pins
    Super::AllocateDefaultPins();

    // Remove input pin — start node cannot have input
    if (UEdGraphPin* InPin = FindPin(FName("In"), EGPD_Input))
    {
        Pins.Remove(InPin);
    }
}

void UConversationGraphStartNode::PostEditUndo()
{
    Super::PostEditUndo();

    ReconstructNode();

    if (UEdGraph* Graph = GetGraph())
    {
        Graph->NotifyGraphChanged();
    }
}

// TSharedRef<SGraphNode> UConversationGraphStartNode::CreateVisualWidget()
// {
//     return SNew(SConversationGraphStartNode, this);
// }
