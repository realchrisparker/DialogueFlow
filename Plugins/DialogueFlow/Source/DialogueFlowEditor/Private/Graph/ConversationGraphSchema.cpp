// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationGraphSchema.cpp
// Description: Graph schema defining rules and behavior for dialogue graphs.
// ============================================================================

#include <Graph/ConversationGraphSchema.h>
#include <Graph/ConversationEdGraph.h>
#include <Graph/Actions/ConversationGraphSchemaAction.h>
#include <Graph/Nodes/ConversationGraphStartNode.h>
#include <Graph/Nodes/ConversationGraphEndNode.h>
#include <Graph/Nodes/ConversationGraphDialogueNode.h>
#include "EdGraph/EdGraph.h"
#include "GraphEditor.h"
#include "GraphEditorActions.h"
#include "ToolMenu.h"
#include "ToolMenuSection.h"
#include "EdGraph/EdGraphNode.h"
#include "Framework/Commands/GenericCommands.h"

#define LOCTEXT_NAMESPACE "DialogueFlowGraphSchema"

void UConversationGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
    AddNodeActions(ContextMenuBuilder);
}

void UConversationGraphSchema::AddNodeActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
    // Add Dialogue Node
    {
        TSharedPtr<FConversationGraphSchemaAction_NewNode> Action =
            MakeShared<FConversationGraphSchemaAction_NewNode>(
                FText::FromString("Dialogue Nodes"),
                FText::FromString("Dialogue"),
                FText::FromString("Create a Dialogue Node."),
                0,
                UConversationGraphDialogueNode::StaticClass()
            );

        ContextMenuBuilder.AddAction(Action);
    }

    // Start Node (commented out to prevent multiple start nodes)
    // {
    //     TSharedPtr<FConversationGraphSchemaAction_NewNode> Action =
    //         MakeShared<FConversationGraphSchemaAction_NewNode>(
    //             FText::FromString("Dialogue Nodes"),
    //             FText::FromString("Start Node"),
    //             FText::FromString("Create a Start Node."),
    //             0,
    //             UConversationGraphStartNode::StaticClass()
    //         );

    //     ContextMenuBuilder.AddAction(Action);
    // }

    // End Node (commented out to prevent multiple end nodes. If we ever add the ability to have multiple start nodes, we should do the same for end nodes)
    // {
    //     TSharedPtr<FConversationGraphSchemaAction_NewNode> Action =
    //         MakeShared<FConversationGraphSchemaAction_NewNode>(
    //             FText::FromString("Dialogue Nodes"),
    //             FText::FromString("End Node"),
    //             FText::FromString("Create an End Node."),
    //             0,
    //             UConversationGraphEndNode::StaticClass()
    //         );

    //     ContextMenuBuilder.AddAction(Action);
    // }
}

void UConversationGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
    if (Context->Node)
    {
        FToolMenuSection& Section = Menu->AddSection("NodeActions", LOCTEXT("NodeActions", "Node Actions"));
        Section.AddMenuEntry("DeleteNode", LOCTEXT("DeleteNode", "Delete"), LOCTEXT("DeleteNode_Tooltip", "Delete this node."),
            FSlateIcon(),
            FUIAction(FExecuteAction::CreateLambda([ Context ] ()
                {
                    if (UEdGraph* Graph = const_cast<UEdGraph*>(Context->Graph.Get()))
                    {
                        if (UEdGraphNode* Node = const_cast<UEdGraphNode*>(Context->Node.Get()))
                        {
                            Graph->RemoveNode(Node);
                        }
                    }
                }))
        );

        FToolMenuSection& AddNodeSection = Menu->AddSection("AddDialogueNode", LOCTEXT("AddDialogueNode", "Dialogue Nodes"));

        AddNodeSection.AddMenuEntry(
            "AddDialogueNodeEntry",
            LOCTEXT("AddDialogueNodeEntry", "Add Dialogue Node"),
            LOCTEXT("AddDialogueNodeEntry_Tooltip", "Create a new Dialogue Node."),
            FSlateIcon(),
            FUIAction(FExecuteAction::CreateLambda([ Context ] ()
                {
                    if (UEdGraph* Graph = const_cast<UEdGraph*>(Context->Graph.Get()))
                    {
                        // Get a good placement location for the new node
                        const FVector2D NodePos = FVector2D(Graph->GetGoodPlaceForNewNode());

                        FGraphNodeCreator<UConversationGraphDialogueNode> Creator(*Graph);
                        UConversationGraphDialogueNode* NewNode = Creator.CreateNode();
                        NewNode->NodePosX = NodePos.X;
                        NewNode->NodePosY = NodePos.Y;
                        Creator.Finalize();
                    }
                }))
        );
    }
}

const FPinConnectionResponse UConversationGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
    // Basic sanity
    if (A == B)
    {
        return FPinConnectionResponse(
            CONNECT_RESPONSE_DISALLOW,
            TEXT("Cannot connect a pin to itself.")
        );
    }

    if (A->Direction == B->Direction)
    {
        return FPinConnectionResponse(
            CONNECT_RESPONSE_DISALLOW,
            TEXT("Pins must be opposite directions.")
        );
    }

    // *** IMPORTANT: use editor graph nodes, not runtime nodes ***
    UConversationGraphNode* NodeA = Cast<UConversationGraphNode>(A->GetOwningNode());
    UConversationGraphNode* NodeB = Cast<UConversationGraphNode>(B->GetOwningNode());

    if (!NodeA || !NodeB)
    {
        return FPinConnectionResponse(
            CONNECT_RESPONSE_DISALLOW,
            TEXT("Only conversation graph nodes are supported.")
        );
    }

    // ---- START NODE RULE ----
    if (NodeA->IsA(UConversationGraphStartNode::StaticClass()) &&
        A->Direction == EGPD_Input)
    {
        return FPinConnectionResponse(
            CONNECT_RESPONSE_DISALLOW,
            TEXT("Start node cannot have inputs.")
        );
    }

    if (NodeB->IsA(UConversationGraphStartNode::StaticClass()) &&
        B->Direction == EGPD_Input)
    {
        return FPinConnectionResponse(
            CONNECT_RESPONSE_DISALLOW,
            TEXT("Start node cannot have inputs.")
        );
    }

    // ---- END NODE RULE ----
    if (NodeA->IsA(UConversationGraphEndNode::StaticClass()) &&
        A->Direction == EGPD_Output)
    {
        return FPinConnectionResponse(
            CONNECT_RESPONSE_DISALLOW,
            TEXT("End nodes cannot have outputs.")
        );
    }

    if (NodeB->IsA(UConversationGraphEndNode::StaticClass()) &&
        B->Direction == EGPD_Output)
    {
        return FPinConnectionResponse(
            CONNECT_RESPONSE_DISALLOW,
            TEXT("End nodes cannot have outputs.")
        );
    }

    // Everything else is fine
    return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

void UConversationGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
    FGraphNodeCreator<UConversationGraphStartNode> Creator(Graph);
    UConversationGraphStartNode* Node = Creator.CreateNode();
    Node->NodePosX = -200;
    Node->NodePosY = 0;
    Creator.Finalize();
}

#undef LOCTEXT_NAMESPACE
