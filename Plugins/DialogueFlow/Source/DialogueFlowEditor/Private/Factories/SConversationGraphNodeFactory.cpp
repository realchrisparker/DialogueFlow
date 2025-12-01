#include "Factories/SConversationGraphNodeFactory.h"
#include "Graph/Nodes/ConversationGraphDialogueNode.h"
#include "Graph/Nodes/ConversationGraphStartNode.h"
#include "Graph/Nodes/ConversationGraphEndNode.h"

#include "Graph/Nodes/SConversationGraphDialogueNode.h"
#include "Graph/Nodes/SConversationGraphStartNode.h"
#include "Graph/Nodes/SConversationGraphEndNode.h"


TSharedPtr<SGraphNode> FConversationGraphNodeFactory::CreateNode(UEdGraphNode* Node) const
{
    if (UConversationGraphDialogueNode* Dialogue = Cast<UConversationGraphDialogueNode>(Node))
    {
        return SNew(SConversationGraphDialogueNode, Dialogue);
    }

    if (UConversationGraphStartNode* Start = Cast<UConversationGraphStartNode>(Node))
    {
        return SNew(SConversationGraphStartNode, Start);
    }

    if (UConversationGraphEndNode* End = Cast<UConversationGraphEndNode>(Node))
    {
        return SNew(SConversationGraphEndNode, End);
    }

    return nullptr;
}
