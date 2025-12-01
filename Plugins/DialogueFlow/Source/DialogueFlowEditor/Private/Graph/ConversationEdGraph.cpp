// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationEdGraph.cpp
// Description: Implementation for the Dialogue Flow editor graph. This file
//              handles Start Node enforcement, schema assignment, and safe
//              undo/redo reconstruction without corrupting custom Slate nodes.
// ============================================================================

#include "Graph/ConversationEdGraph.h"
#include "Graph/ConversationGraphSchema.h"
#include "Graph/Nodes/ConversationGraphStartNode.h"
#include "Graph/Nodes/ConversationGraphEndNode.h"
#include "Nodes/DialogueFlowBaseNode.h"
#include "EdGraph/EdGraphNode.h"

UConversationEdGraph::UConversationEdGraph()
{
	// Always set our custom schema
	Schema = UConversationGraphSchema::StaticClass();
}


// START NODE LOOKUP


UConversationGraphStartNode* UConversationEdGraph::FindStartNode() const
{
	for (UEdGraphNode* Node : Nodes)
	{
		if (UConversationGraphStartNode* Start = Cast<UConversationGraphStartNode>(Node))
		{
			return Start;
		}
	}
	return nullptr;
}

void UConversationEdGraph::EnsureStartNodeExists()
{
	// Search first
	if (FindStartNode())
	{
		return; // Exists, do nothing
	}

	// Create new start node
	FGraphNodeCreator<UConversationGraphStartNode> Creator(*this);
	UConversationGraphStartNode* NewStart = Creator.CreateNode(/*bSelectNewNode=*/ false);
	NewStart->NodePosX = -200;
	NewStart->NodePosY = 0;
	Creator.Finalize();

	Modify();
}

// POST LOAD
void UConversationEdGraph::PostLoad()
{
	Super::PostLoad();

	// Ensure schema is valid
	if (!Schema)
	{
		Schema = UConversationGraphSchema::StaticClass();
	}

	// Ensure start node exists, but DO NOT reconstruct pins here
	EnsureStartNodeExists();

	EnsureRequiredNodesExist();

	// No ReconstructNode() here!
	// Slate visual nodes are not ready during PostLoad.
	// Doing so would destroy all custom pin widgets & context menus.
}

// VALIDATION
void UConversationEdGraph::ValidateGraphSafe()
{
	// Only minimal non-destructive checks
	EnsureStartNodeExists();
}


// SYNC RUNTIME DATA
void UConversationEdGraph::SyncEditorGraphToRuntime()
{
	// Reset runtime links
	for (UEdGraphNode* Node : Nodes)
	{
		if (UConversationGraphNode* CNode = Cast<UConversationGraphNode>(Node))
		{
			if (UDialogueFlowBaseNode* Data = CNode->GetNodeData())
			{
				Data->InputLinks.Empty();
				Data->OutputLinks.Empty();
			}
		}
	}

	// Rebuild runtime links
	for (UEdGraphNode* Node : Nodes)
	{
		UConversationGraphNode* CNode = Cast<UConversationGraphNode>(Node);
		if (!CNode)
			continue;

		UDialogueFlowBaseNode* Runtime = CNode->GetNodeData();
		if (!Runtime)
			continue;

		const int32 ThisNodeID = Runtime->NodeID;

		for (UEdGraphPin* Pin : CNode->Pins)
		{
			if (Pin->Direction != EGPD_Output)
				continue;

			for (UEdGraphPin* Linked : Pin->LinkedTo)
			{
				if (!Linked)
					continue;

				if (UConversationGraphNode* TargetGraphNode = Cast<UConversationGraphNode>(Linked->GetOwningNode()))
				{
					if (UDialogueFlowBaseNode* TargetRuntime = TargetGraphNode->GetNodeData())
					{
						Runtime->OutputLinks.Add(TargetRuntime->NodeID);
						TargetRuntime->InputLinks.Add(ThisNodeID);
					}
				}
			}
		}
	}
}


// UNDO / REDO
void UConversationEdGraph::PostEditUndo()
{
	Super::PostEditUndo();

	// Ensure Start Node never disappears
	EnsureStartNodeExists();

	// NOW it's safe to reconstruct nodes
	for (UEdGraphNode* Node : Nodes)
	{
		if (Node)
		{
			Node->ReconstructNode();
		}
	}

	// Notify UI to refresh
	NotifyGraphChanged();

	EnsureRequiredNodesExist();
}

void UConversationEdGraph::EnsureRequiredNodesExist()
{
	bool bHasStart = false;
	bool bHasEnd = false;

	for (UEdGraphNode* Node : Nodes)
	{
		if (Cast<UConversationGraphStartNode>(Node))
			bHasStart = true;

		if (Cast<UConversationGraphEndNode>(Node))
			bHasEnd = true;
	}

	if (!bHasStart)
	{
		FGraphNodeCreator<UConversationGraphStartNode> Creator(*this);
		UConversationGraphStartNode* Start = Creator.CreateNode(false);
		Start->NodePosX = -200;
		Start->NodePosY = 0;
		Creator.Finalize();
	}

	if (!bHasEnd)
	{
		FGraphNodeCreator<UConversationGraphEndNode> Creator(*this);
		UConversationGraphEndNode* End = Creator.CreateNode(false);
		End->NodePosX = 400;
		End->NodePosY = 0;
		Creator.Finalize();
	}
}
