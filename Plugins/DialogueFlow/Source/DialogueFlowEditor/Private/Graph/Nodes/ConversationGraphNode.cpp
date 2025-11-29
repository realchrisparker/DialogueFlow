// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationGraphNode.cpp
// Description: Base editor graph node for dialogue flow.
// ============================================================================

#include <Graph/Nodes/ConversationGraphNode.h>
#include "EdGraph/EdGraphPin.h"


// Static pin names
const FName UConversationGraphNode::PinInput = TEXT("In");
const FName UConversationGraphNode::PinOutput = TEXT("Out");

void UConversationGraphNode::AllocateDefaultPins()
{
    // Default graph nodes have both an input and output pin
    CreatePin(EGPD_Input, TEXT("DialogueFlow"), PinInput);
    CreatePin(EGPD_Output, TEXT("DialogueFlow"), PinOutput);
}

FText UConversationGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    if (DialogueNode)
    {
        return DialogueNode->NodeTitle.IsEmpty()
            ? FText::FromString(TEXT("Dialogue Node"))
            : DialogueNode->NodeTitle;
    }

    return FText::FromString(TEXT("Dialogue Node"));
}

void UConversationGraphNode::PrepareForCopying()
{
    Super::PrepareForCopying();

    // Ensure runtime object is included in graph editor copy operation
    if (DialogueNode)
    {
        DialogueNode->SetFlags(RF_Transactional);
        DialogueNode->Rename(nullptr, this, REN_DontCreateRedirectors);
    }
}
