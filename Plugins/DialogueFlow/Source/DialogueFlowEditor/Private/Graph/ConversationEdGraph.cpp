// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationEdGraph.cpp
// Description: Base graph class for Dialogue Flow editor graphs.
// ============================================================================

#include <Graph/ConversationEdGraph.h>
#include <Graph/ConversationGraphSchema.h>
#include <Nodes/DialogueFlowStartNode.h>
#include <Nodes/DialogueFlowDialogueNode.h> 
#include <Graph/Nodes/ConversationGraphDialogueNode.h>
#include <Graph/Nodes/ConversationGraphStartNode.h>
#include <Graph/Nodes/ConversationGraphEndNode.h>


UConversationEdGraph::UConversationEdGraph()
{
	Schema = UConversationGraphSchema::StaticClass();
	StartNode = nullptr;
}

void UConversationEdGraph::PostLoad()
{
	Super::PostLoad();
	RebuildGraph();
	ValidateGraph();
}

void UConversationEdGraph::PostEditUndo()
{
	Super::PostEditUndo();
	RebuildGraph();
	ValidateGraph();
}

void UConversationEdGraph::RebuildGraph()
{
    StartNode = nullptr;

    for (UEdGraphNode* Node : Nodes)
    {
        if (UConversationGraphStartNode* Start = Cast<UConversationGraphStartNode>(Node))
        {
            StartNode = Start;
            break;
        }
    }

    if (StartNode == nullptr)
    {
        FGraphNodeCreator<UConversationGraphStartNode> Creator(*this);
        UConversationGraphStartNode* NewStart = Creator.CreateNode();
        NewStart->NodePosX = -200;
        NewStart->NodePosY = 0;
        Creator.Finalize();

        StartNode = NewStart;
        Modify();
    }

    // Reconstruct nodes to rebuild their pins from data
    for (UEdGraphNode* Node : Nodes)
    {
        if (Node)
        {
            Node->ReconstructNode();
        }
    }
}

void UConversationEdGraph::ValidateGraph()
{
    const UEdGraphSchema* schema = GetSchema();
    if (!schema)
    {
        return;
    }
    
    // 1. Remove null nodes or trashed nodes    
	for (int32 i = Nodes.Num() - 1; i >= 0; --i)
	{
		if (Nodes[i] == nullptr)
		{
			Nodes.RemoveAt(i);
		}
	}
    
    // 2. Ensure exactly one Start Node exists    
    UConversationGraphStartNode* FoundStart = nullptr;

    for (UEdGraphNode* Node : Nodes)
    {
        if (auto* Start = Cast<UConversationGraphStartNode>(Node))
        {
            if (FoundStart == nullptr)
            {
                FoundStart = Start;
            }
            else
            {
                // Extra start node found – delete it
                RemoveNode(Node);
            }
        }
    }

    // If no Start Node exists, create one
    if (FoundStart == nullptr)
    {
        FGraphNodeCreator<UConversationGraphStartNode> Creator(*this);
        UConversationGraphStartNode* NewStart = Creator.CreateNode();
        NewStart->NodePosX = -200;
        NewStart->NodePosY = 0;
        Creator.Finalize();

        FoundStart = NewStart;
    }

    StartNode = FoundStart;

    // Ensure exactly one End Node exists
    UConversationGraphEndNode* FoundEnd = nullptr;

    for (UEdGraphNode* Node : Nodes)
    {
        if (auto* End = Cast<UConversationGraphEndNode>(Node))
        {
            if (FoundEnd == nullptr)
            {
                FoundEnd = End;
            }
            else
            {
                // Delete duplicate end node
                RemoveNode(Node);
            }
        }
    }

    // If no End Node exists, create one
    if (FoundEnd == nullptr)
    {
        FGraphNodeCreator<UConversationGraphEndNode> Creator(*this);
        UConversationGraphEndNode* NewEnd = Creator.CreateNode();
        NewEnd->NodePosX = 200;
        NewEnd->NodePosY = 0;
        Creator.Finalize();

        FoundEnd = NewEnd;
    }

    // Dialogue Node Validation
    ValidateDialogueNodes();

    // Enforce single-output rule for choice pins
    FixDialogueConnections();

    // Break cycles (schema blocks creation, but this cleans legacy) 
    BreakCycles();

    // 3. Enforce Start Node pin rules: NO inputs
    if (StartNode)
    {
        for (UEdGraphPin* Pin : StartNode->Pins)
        {
            if (Pin->Direction == EGPD_Input)
            {
                Pin->BreakAllPinLinks();
            }
        }
    }

    // 4. Enforce End Node pin rules: NO outputs
    for (UEdGraphNode* Node : Nodes)
    {
        if (auto* EndNode = Cast<UConversationGraphEndNode>(Node))
        {
            for (UEdGraphPin* Pin : EndNode->Pins)
            {
                if (Pin->Direction == EGPD_Output)
                {
                    Pin->BreakAllPinLinks();
                }
            }
        }
    }

    // 5. Validate ALL links according to schema rules
    for (UEdGraphNode* Node : Nodes)
    {
        for (UEdGraphPin* Pin : Node->Pins)
        {
            // Copy array because we might remove during iteration
            TArray<UEdGraphPin*> LinkedToCopy = Pin->LinkedTo;

            for (UEdGraphPin* Linked : LinkedToCopy)
            {
                if (!Linked) 
                {
                    Pin->LinkedTo.Remove(Linked);
                    continue;
                }

                const FPinConnectionResponse Response = schema->CanCreateConnection(Pin, Linked);

                if (Response.Response == CONNECT_RESPONSE_DISALLOW)
                {
                    Pin->BreakLinkTo(Linked);
                }
            }
        }
    }
}

void UConversationEdGraph::ValidateDialogueNodes()
{
    for (UEdGraphNode* Node : Nodes)
    {
        if (auto* GraphDialogueNode = Cast<UConversationGraphDialogueNode>(Node))
        {
            UDialogueFlowDialogueNode* Runtime = GraphDialogueNode->GetDialogueNode();
            if (!Runtime)
                continue;

            // DialogueText must not be empty
            if (Runtime->DialogueText.IsEmpty())
            {
                // You can enhance with error badges later
                UE_LOG(LogTemp, Warning,
                    TEXT("Dialogue node at %s has empty DialogueText."),
                    *GraphDialogueNode->GetName());
            }

            // Each choice must have a title
            for (int32 i = 0; i < Runtime->Choices.Num(); ++i)
            {
                if (Runtime->Choices[i].ChoiceTitle.IsEmpty())
                {
                    UE_LOG(LogTemp, Warning,
                        TEXT("Dialogue node '%s' has a choice missing a title (index %d)."),
                        *GraphDialogueNode->GetName(), i);
                }
            }
        }
    }
}

void UConversationEdGraph::FixDialogueConnections()
{
    for (UEdGraphNode* Node : Nodes)
    {
        if (auto* GraphDialogueNode = Cast<UConversationGraphDialogueNode>(Node))
        {
            for (UEdGraphPin* Pin : Node->Pins)
            {
                if (Pin->Direction == EGPD_Output)
                {
                    // Enforce ONE connection rule
                    while (Pin->LinkedTo.Num() > 1)
                    {
                        UEdGraphPin* Extra = Pin->LinkedTo.Last();
                        Pin->BreakLinkTo(Extra);
                    }
                }
            }
        }
    }
}

bool UConversationEdGraph::HasPathToNode(UEdGraphNode* Start, UEdGraphNode* Target) const
{
    TSet<UEdGraphNode*> Visited;
    TArray<UEdGraphNode*> Stack;
    Stack.Add(Start);

    while (!Stack.IsEmpty())
    {
        UEdGraphNode* Current = Stack.Pop();
        if (Current == Target)
            return true;

        Visited.Add(Current);

        for (UEdGraphPin* Pin : Current->Pins)
        {
            if (Pin->Direction == EGPD_Output)
            {
                for (UEdGraphPin* Linked : Pin->LinkedTo)
                {
                    UEdGraphNode* Next = Linked->GetOwningNode();
                    if (!Visited.Contains(Next))
                    {
                        Stack.Add(Next);
                    }
                }
            }
        }
    }

    return false;
}

void UConversationEdGraph::BreakCycles()
{
    for (UEdGraphNode* Node : Nodes)
    {
        for (UEdGraphPin* Pin : Node->Pins)
        {
            if (Pin->Direction == EGPD_Output)
            {
                TArray<UEdGraphPin*> Copy = Pin->LinkedTo;

                for (UEdGraphPin* Linked : Copy)
                {
                    UEdGraphNode* Target = Linked->GetOwningNode();

                    if (HasPathToNode(Target, Node))  // cycle
                    {
                        UE_LOG(LogTemp, Warning,
                            TEXT("Breaking cycle between '%s' and '%s'."),
                            *Node->GetName(),
                            *Target->GetName());

                        Pin->BreakLinkTo(Linked);
                    }
                }
            }
        }
    }
}
