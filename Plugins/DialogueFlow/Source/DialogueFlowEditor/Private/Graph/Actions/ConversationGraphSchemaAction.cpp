
#include <Graph/Actions/ConversationGraphSchemaAction.h>
#include <Graph/Nodes/ConversationGraphNode.h>
#include <Graph/Nodes/ConversationGraphDialogueNode.h>
#include <Graph/ConversationEdGraph.h>
#include <Nodes/DialogueFlowBaseNode.h>
#include <Nodes/DialogueFlowDialogueNode.h>
#include <Assets/ConversationAsset.h>
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
    if (!ParentGraph || !GraphNodeClass)
    {
        return nullptr;
    }

    const FScopedTransaction Transaction(
        NSLOCTEXT("ConversationGraph", "AddDialogueNode", "Add Dialogue Node")
    );

    ParentGraph->Modify();

    // ------------------------------
    // Get the owning ConversationAsset
    // ------------------------------
    UConversationAsset* ConversationAsset = Cast<UConversationAsset>(ParentGraph->GetOuter());
    if (!ConversationAsset)
    {
        return nullptr;
    }

    ConversationAsset->Modify();

    // ------------------------------
    // Create the graph node
    // ------------------------------
    FGraphNodeCreator<UConversationGraphDialogueNode> Creator(*ParentGraph);
    UConversationGraphDialogueNode* NewGraphNode = Creator.CreateNode(bSelectNewNode);

    if (!NewGraphNode)
    {
        Creator.Finalize();
        return nullptr;
    }

    NewGraphNode->NodePosX = Location.X;
    NewGraphNode->NodePosY = Location.Y;

    NewGraphNode->AutowireNewNode(FromPin);
    NewGraphNode->AllocateDefaultPins();

    Creator.Finalize();

    // ------------------------------
    // Create the runtime dialogue node
    // ------------------------------
    UDialogueFlowDialogueNode* NewRuntimeNode =
        NewObject<UDialogueFlowDialogueNode>(
            ConversationAsset,
            UDialogueFlowDialogueNode::StaticClass(),
            NAME_None,
            RF_Transactional
        );

    if (NewRuntimeNode)
    {
        // Add runtime node to asset
        int32 NewIndex = ConversationAsset->Nodes.Add(NewRuntimeNode);

        // Give it a default title
        NewRuntimeNode->NodeTitle = FText::FromString(TEXT("Dialogue Node"));

        // Link graph node ←→ runtime node
        NewGraphNode->SetNodeData(NewRuntimeNode);

        if (NewRuntimeNode)
        {
            NewRuntimeNode->PropertyChangedDelegate.AddUObject(
                NewGraphNode,
                &UConversationGraphDialogueNode::HandleRuntimeNodePropertyChanged_Internal
            );
        }
    }

    ParentGraph->NotifyGraphChanged();

    return NewGraphNode;
}
