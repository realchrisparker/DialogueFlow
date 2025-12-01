// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationGraphDialogueNode.h
// Description: Editor Dialogue node. Supports dynamic output pins that
//              correspond to runtime dialogue choices.
// ============================================================================

#include "Graph/Nodes/ConversationGraphDialogueNode.h"
#include "Nodes/DialogueFlowDialogueNode.h"
#include "EdGraph/EdGraphPin.h"

UConversationGraphDialogueNode::UConversationGraphDialogueNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeClassName = "Dialogue";
}

void UConversationGraphDialogueNode::AllocateDefaultPins()
{
	Pins.Reset();

	// Input pin (always one)
	CreatePin(EGPD_Input, TEXT("DialogueFlow"), FName("In"));

	// Runtime node controls how many output pins exist
	UDialogueFlowDialogueNode* Runtime = GetDialogueNode();
	const int32 NumChoices = Runtime ? Runtime->Choices.Num() : 0;

	for (int32 i = 0; i < NumChoices; i++)
	{
		const FText& Title = Runtime->Choices[i].ChoiceTitle;

		FName PinName = Title.IsEmpty()
			? FName(*FString::Printf(TEXT("Choice_%d"), i))
			: FName(*Title.ToString());

		CreatePin(EGPD_Output, TEXT("DialogueFlow"), PinName);
	}
}

void UConversationGraphDialogueNode::ReconstructNode()
{
	TArray<UEdGraphPin*> OldPins = Pins;

	AllocateDefaultPins();

	// Base restore ensures GUID match restoration
	Super::ReconstructNode();

	// Also restore dynamic connections
	RestoreDynamicConnections(OldPins);

	// Synchronize pin names to choice text
	SyncPinNamesToChoices();
}

void UConversationGraphDialogueNode::PostEditUndo()
{
	Super::PostEditUndo();

	ReconstructNode();

	if (UEdGraph* Graph = GetGraph())
	{
		Graph->NotifyGraphChanged();
	}
}

UDialogueFlowDialogueNode* UConversationGraphDialogueNode::GetDialogueNode() const
{
	return Cast<UDialogueFlowDialogueNode>(GetNodeData());
}

void UConversationGraphDialogueNode::RestoreDynamicConnections(const TArray<UEdGraphPin*>& OldPins)
{
	TMap<FGuid, UEdGraphPin*> OldMap;
	for (UEdGraphPin* Old : OldPins)
	{
		if (Old)
			OldMap.Add(Old->PersistentGuid, Old);
	}

	for (const FConversationSavedPinLink& Saved : SavedConnectionData.SavedLinks)
	{
		UEdGraphPin* ThisPin = nullptr;
		UEdGraphPin* OtherPin = nullptr;

		for (UEdGraphPin* Pin : Pins)
		{
			if (Pin->PersistentGuid == Saved.ThisPinId)
				ThisPin = Pin;
			if (Pin->PersistentGuid == Saved.LinkedPinId)
				OtherPin = Pin;
		}

		if (ThisPin && OtherPin)
		{
			ThisPin->MakeLinkTo(OtherPin);
		}
	}
}

void UConversationGraphDialogueNode::SyncPinNamesToChoices()
{
	UDialogueFlowDialogueNode* Runtime = GetDialogueNode();
	if (!Runtime) return;

	int32 OutIndex = 0;

	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->Direction == EGPD_Output && Runtime->Choices.IsValidIndex(OutIndex))
		{
			const FText& Title = Runtime->Choices[OutIndex].ChoiceTitle;

			Pin->PinName = Title.IsEmpty()
				? FName(*FString::Printf(TEXT("Choice_%d"), OutIndex))
				: FName(*Title.ToString());

			OutIndex++;
		}
	}
}
