// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationGraphEndNode.h
// Description: Editor End node. One input pin, no outputs.
// ============================================================================

#include "Graph/Nodes/ConversationGraphEndNode.h"
#include "EdGraph/EdGraphPin.h"

UConversationGraphEndNode::UConversationGraphEndNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeClassName = "End";
}

void UConversationGraphEndNode::AllocateDefaultPins()
{
	// Base class creates In + Out pins
	Super::AllocateDefaultPins();

	// Remove output pin — end node never has outputs
	if (UEdGraphPin* OutPin = FindPin(FName("Out"), EGPD_Output))
	{
		Pins.Remove(OutPin);
	}
}

void UConversationGraphEndNode::PostEditUndo()
{
	Super::PostEditUndo();

	ReconstructNode();

	if (UEdGraph* Graph = GetGraph())
	{
		Graph->NotifyGraphChanged();
	}
}

// TSharedRef<SGraphNode> UConversationGraphEndNode::CreateVisualWidget()
// {
// 	return SNew(SConversationGraphEndNode, this);
// }
