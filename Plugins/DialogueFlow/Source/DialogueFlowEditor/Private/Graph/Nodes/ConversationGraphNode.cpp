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


UConversationGraphNode::UConversationGraphNode(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    CreateNewGuid(); // Ensure each node has a unique identifier
}

void UConversationGraphNode::AllocateDefaultPins()
{
    // let parent class clear pins
    Pins.Reset();

    // input
    CreatePin(EGPD_Input, TEXT("DialogueFlow"), PinInput);

    // output
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

void UConversationGraphNode::ReconstructNode()
{
    // Save old pins for link restoration
    TArray<UEdGraphPin*> OldPins = Pins;

    // Clear and rebuild the pins
    AllocateDefaultPins();

    // Attempt to reconnect old links using pin name/direction
    for (UEdGraphPin* OldPin : OldPins)
    {
        if (!OldPin) continue;

        for (UEdGraphPin* NewPin : Pins)
        {
            const bool bMatchDir  = (NewPin->Direction == OldPin->Direction);
            const bool bMatchName = (NewPin->PinName   == OldPin->PinName);

            if (bMatchDir && bMatchName)
            {
                NewPin->MovePersistentDataFromOldPin(*OldPin);
                break;
            }
        }
    }
}
