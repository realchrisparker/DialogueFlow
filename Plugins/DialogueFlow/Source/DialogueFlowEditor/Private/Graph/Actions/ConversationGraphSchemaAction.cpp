#include <Graph/Actions/ConversationGraphSchemaAction.h>
#include <Graph/Nodes/ConversationGraphNode.h>
#include "EdGraph/EdGraph.h"


/**
 * Performs the action of creating a new node in the graph.
 */
UEdGraphNode* FConversationGraphSchemaAction_NewNode::PerformAction(
    UEdGraph* ParentGraph,
    UEdGraphPin* FromPin,
    const FVector2D Location,
    bool bSelectNewNode)
{
    if (!GraphNodeClass || !ParentGraph)
        return nullptr;

    // Create the node object as a child of the graph
    UEdGraphNode* NewNode = NewObject<UEdGraphNode>(
        ParentGraph,
        GraphNodeClass,
        NAME_None,
        RF_Transactional
    );

    // Add to graph structure
    ParentGraph->AddNode(NewNode, bSelectNewNode, bSelectNewNode);

    // Set position
    NewNode->NodePosX = Location.X;
    NewNode->NodePosY = Location.Y;

    // Allocate default pins
    NewNode->AllocateDefaultPins();

    // Auto-wire output → input
    if (FromPin && FromPin->Direction == EGPD_Output)
    {
        if (UEdGraphPin* InputPin = NewNode->FindPin(TEXT("In")))
        {
            FromPin->MakeLinkTo(InputPin);
        }
    }

    return NewNode;
}


