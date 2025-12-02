// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationAssetFactory.cpp
// Description: Implementation for Conversation Asset Factory.
// ============================================================================

#include <Factories/ConversationAssetFactory.h>
#include <Graph/ConversationEdGraph.h>
#include <Graph/ConversationGraphSchema.h>
#include <Graph/Nodes/ConversationGraphStartNode.h>
#include <Graph/Nodes/ConversationGraphEndNode.h>
#include <Assets/ConversationAsset.h>
#include <Nodes/DialogueFlowStartNode.h>
#include <Nodes/DialogueFlowEndNode.h>
#include <Nodes/DialogueFlowDialogueNode.h>

/*
 * Constructor
 */
UConversationAssetFactory::UConversationAssetFactory()
{
    SupportedClass = UConversationAsset::StaticClass();
    bCreateNew = true;
    bEditAfterNew = true;
}

/*
 * Creates a new instance of a Conversation Asset
 */
UObject* UConversationAssetFactory::FactoryCreateNew(
    UClass* Class,
    UObject* InParent,
    FName Name,
    EObjectFlags Flags,
    UObject* Context,
    FFeedbackContext* Warn
)
{
    // Create the asset
    UConversationAsset* NewAsset = NewObject<UConversationAsset>(InParent, Class, Name, Flags);
    NewAsset->Modify();

    // Create the Editor Graph
    UConversationEdGraph* Graph = NewObject<UConversationEdGraph>(
        NewAsset,
        UConversationEdGraph::StaticClass(),
        NAME_None,
        RF_Transactional
    );
    Graph->Schema = UConversationGraphSchema::StaticClass();
    Graph->Modify();

    // Assign to asset
    NewAsset->EditorGraph = Graph;

    // CREATE REQUIRED START NODE
    {
        FGraphNodeCreator<UConversationGraphStartNode> Creator(*Graph);
        UConversationGraphStartNode* StartNode = Creator.CreateNode(false);
        StartNode->NodePosX = -200;
        StartNode->NodePosY = 0;

        UDialogueFlowStartNode* StartData = NewObject<UDialogueFlowStartNode>(NewAsset, UDialogueFlowStartNode::StaticClass(), NAME_None, RF_Transactional);
        NewAsset->Nodes.Add(StartData);
        StartNode->SetNodeData(StartData);

        Creator.Finalize();
    }

    // CREATE REQUIRED END NODE
    {
        FGraphNodeCreator<UConversationGraphEndNode> Creator(*Graph);
        UConversationGraphEndNode* EndNode = Creator.CreateNode(false);
        EndNode->NodePosX = 400;
        EndNode->NodePosY = 0;

        UDialogueFlowEndNode* EndData = NewObject<UDialogueFlowEndNode>(NewAsset, UDialogueFlowEndNode::StaticClass(), NAME_None, RF_Transactional);
        NewAsset->Nodes.Add(EndData);
        EndNode->SetNodeData(EndData);

        Creator.Finalize();
    }

    return NewAsset;
}
