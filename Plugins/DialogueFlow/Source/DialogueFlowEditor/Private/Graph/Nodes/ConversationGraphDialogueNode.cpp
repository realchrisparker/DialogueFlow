// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationGraphDialogueNode.cpp
// Description: Editor graph node implementation for Dialogue nodes.
// ============================================================================

#include <Graph/Nodes/ConversationGraphDialogueNode.h>
#include <Nodes/DialogueFlowDialogueNode.h>
#include <Graph/Nodes/SConversationGraphDialogueNode.h>
#include "EdGraph/EdGraphPin.h"


#define LOCTEXT_NAMESPACE "ConversationGraphNodes"

UConversationGraphDialogueNode::UConversationGraphDialogueNode()
{
}

UDialogueFlowDialogueNode* UConversationGraphDialogueNode::GetOrCreateDialogueNode()
{
    // If we already have a runtime node, return it.
    if (DialogueNode)
    {
        return Cast<UDialogueFlowDialogueNode>(DialogueNode);
    }

    // Lazily create a new runtime dialogue node as a child of this graph node.
    // Being referenced by the UPROPERTY DialogueNode ensures it is serialized
    // and garbage collected correctly with the graph.
    UDialogueFlowDialogueNode* NewRuntimeNode = NewObject<UDialogueFlowDialogueNode>(
        this,
        UDialogueFlowDialogueNode::StaticClass(),
        NAME_None,
        RF_Transactional
    );

    DialogueNode = NewRuntimeNode;
    return NewRuntimeNode;
}

UDialogueFlowDialogueNode* UConversationGraphDialogueNode::GetDialogueNode() const
{
    // Delegate to the non-const helper that guarantees creation.
    return const_cast<UConversationGraphDialogueNode*>(this)->GetOrCreateDialogueNode();
}

UObject* UConversationGraphDialogueNode::GetPropertyObject() const
{
    // For details panel selection, always expose the runtime dialogue node.
    if (UDialogueFlowDialogueNode* RuntimeNode = GetDialogueNode())
    {
        return RuntimeNode;
    }

    // Fallback (should not normally be hit now, but keeps the contract sane).
    return Super::GetPropertyObject();
}

void UConversationGraphDialogueNode::AllocateDefaultPins()
{
    UDialogueFlowDialogueNode* DNode = GetDialogueNode();
    Pins.Empty();

    // ---- Input Pin ---------------------------------------------------------
    CreatePin(EGPD_Input, TEXT("DialogueFlow"), PinInput);

    // ---- Output Pins --------------------------------------------------------
    if (DNode)
    {
        for (int32 i = 0; i < DNode->Choices.Num(); i++)
        {
            const FText& Title = DNode->Choices[i].ChoiceTitle;
            const FString PinName = Title.IsEmpty()
                ? FString::Printf(TEXT("Choice %d"), i)
                : Title.ToString();

            UEdGraphPin* NewPin = CreatePin(EGPD_Output, TEXT("DialogueFlow"), *PinName);
            DNode->Choices[i].LinkedOutputPinIndex = i;
        }
    }
}

void UConversationGraphDialogueNode::RebuildOutputPins()
{
    // Remove old output pins
    TArray<UEdGraphPin*> PinsToRemove;
    for (UEdGraphPin* Pin : Pins)
    {
        if (Pin->Direction == EGPD_Output)
        {
            PinsToRemove.Add(Pin);
        }
    }
    for (UEdGraphPin* Pin : PinsToRemove)
    {
        RemovePin(Pin);
    }

    // Regenerate output pins
    UDialogueFlowDialogueNode* DNode = GetDialogueNode();
    if (!DNode)
        return;

    for (int32 i = 0; i < DNode->Choices.Num(); i++)
    {
        const FText& Title = DNode->Choices[i].ChoiceTitle;
        const FString PinName = Title.IsEmpty()
            ? FString::Printf(TEXT("Choice %d"), i)
            : Title.ToString();

        UEdGraphPin* NewPin = CreatePin(EGPD_Output, TEXT("DialogueFlow"), *PinName);
        DNode->Choices[i].LinkedOutputPinIndex = i;
    }

    GetGraph()->NotifyNodeChanged(this);
}

void UConversationGraphDialogueNode::SyncChoicePinIndices()
{
    UDialogueFlowDialogueNode* DNode = GetDialogueNode();
    if (!DNode)
        return;

    int32 OutputIndex = 0;

    for (UEdGraphPin* Pin : Pins)
    {
        if (Pin->Direction == EGPD_Output)
        {
            if (DNode->Choices.IsValidIndex(OutputIndex))
            {
                DNode->Choices[OutputIndex].LinkedOutputPinIndex = OutputIndex;
            }
            OutputIndex++;
        }
    }
}

void UConversationGraphDialogueNode::PinConnectionListChanged(UEdGraphPin* Pin)
{
    Super::PinConnectionListChanged(Pin);
}

void UConversationGraphDialogueNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.Property)
    {
        const FName PropertyName = PropertyChangedEvent.Property->GetFName();

        // If the Choices array changed...
        if (PropertyName == GET_MEMBER_NAME_CHECKED(UDialogueFlowDialogueNode, Choices))
        {
            RebuildOutputPins();
            SyncChoicePinIndices();
        }
    }
}

FText UConversationGraphDialogueNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
    UDialogueFlowDialogueNode* DNode = GetDialogueNode();

    if (DNode && !DNode->SpeakerName.IsEmpty())
    {
        return FText::Format(
            LOCTEXT("DialogueNodeTitle", "{0}: Dialogue"),
            DNode->SpeakerName
        );
    }

    return LOCTEXT("DialogueNodeTitle_NoSpeaker", "Dialogue");
}

FLinearColor UConversationGraphDialogueNode::GetNodeTitleColor() const
{
    return FLinearColor(0.20f, 0.25f, 0.80f); // Matches runtime node color
}

FText UConversationGraphDialogueNode::GetTooltipText() const
{
    return LOCTEXT("DialogueNodeTooltip", "NPC line and branching dialogue choices.");
}

TSharedPtr<SGraphNode> UConversationGraphDialogueNode::CreateVisualWidget()
{
    return SNew(SConversationGraphDialogueNode, this);
}

#undef LOCTEXT_NAMESPACE
