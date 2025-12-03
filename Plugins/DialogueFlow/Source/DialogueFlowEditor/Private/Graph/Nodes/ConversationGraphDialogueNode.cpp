// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationGraphDialogueNode.cpp
// Description: Editor dialogue node implementation. Allocates one input pin and
//              one dynamic output pin per runtime dialogue choice.
// ============================================================================

#include "Graph/Nodes/ConversationGraphDialogueNode.h"

#include "Nodes/DialogueFlowDialogueNode.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphPin.h"

#define LOCTEXT_NAMESPACE "ConversationGraphDialogueNode"

/**
 * Default constructor.
 *
 * Initializes the base node and sets a class name string used for debugging
 * and potential styling in Slate widgets.
 */
UConversationGraphDialogueNode::UConversationGraphDialogueNode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeClassName = TEXT("Dialogue");
}

/**
 * Allocates this node's pins.
 *
 * We always create:
 * - One input pin named "In".
 * - One output pin per choice defined on the runtime dialogue node.
 */
void UConversationGraphDialogueNode::AllocateDefaultPins()
{
	Pins.Reset();

	// Single execution/flow input pin.
	CreatePin(EGPD_Input, TEXT("DialogueFlow"), TEXT("In"));

	// Dynamic output pins based on the runtime dialogue node.
	UDialogueFlowDialogueNode* Runtime = GetDialogueNode();
	const int32 NumChoices = Runtime ? Runtime->Choices.Num() : 0;

	for (int32 ChoiceIdx = 0; ChoiceIdx < NumChoices; ++ChoiceIdx)
	{
		FName PinName = NAME_None;

		if (Runtime && Runtime->Choices.IsValidIndex(ChoiceIdx))
		{
			const FText& Title = Runtime->Choices[ChoiceIdx].ChoiceTitle;

			PinName = Title.IsEmpty()
				? FName(*FString::Printf(TEXT("Choice_%d"), ChoiceIdx))
				: FName(*Title.ToString());
		}
		else
		{
			PinName = FName(*FString::Printf(TEXT("Choice_%d"), ChoiceIdx));
		}

		CreatePin(EGPD_Output, TEXT("DialogueFlow"), PinName);
	}
}

/**
 * Rebuilds pins and attempts to restore dynamic connections.
 *
 * We capture the existing pins, let the base class rebuild (which calls
 * AllocateDefaultPins), then restore dynamic output connections and sync
 * display names to the runtime choices.
 */
void UConversationGraphDialogueNode::ReconstructNode()
{
	// Save old pins so we can rewire dynamic edges after reconstruction.
	TArray<UEdGraphPin*> OldPins = Pins;

	// Let the base class rebuild pins and static connections.
	Super::ReconstructNode();

	// Restore dynamic output connections by matching pin names.
	RestoreDynamicConnections(OldPins);

	// Make sure pin names still match the runtime choices.
	SyncPinNamesToChoices();
}

/**
 * Returns the node title text shown in the graph.
 *
 * Uses the runtime node's NodeTitle when available, otherwise returns a
 * localized "Dialogue" label.
 */
FText UConversationGraphDialogueNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (const UDialogueFlowDialogueNode* Runtime = GetDialogueNode())
	{
		if (!Runtime->NodeTitle.IsEmpty())
		{
			return Runtime->NodeTitle;
		}
	}

	return LOCTEXT("DialogueNodeTitle", "Dialogue");
}

/**
 * Ensures the node is correctly rebuilt after undo/redo operations.
 *
 * We rely on the base implementation to call ReconstructNode and notify the
 * graph; this override exists to preserve future extension points while
 * documenting the behavior.
 */
void UConversationGraphDialogueNode::PostEditUndo()
{
	Super::PostEditUndo();
}

/**
 * Returns the runtime dialogue node that this editor node is bound to.
 *
 * The base UConversationGraphNode already manages a UDialogueFlowBaseNode*
 * pointer; this simply casts it to the concrete UDialogueFlowDialogueNode type.
 */
UDialogueFlowDialogueNode* UConversationGraphDialogueNode::GetDialogueNode() const
{
	return Cast<UDialogueFlowDialogueNode>(GetNodeData());
}

/**
 * Restores dynamic output connections after pins have been reconstructed.
 *
 * We match old and new pins by PinName and recreate any links found on the
 * old pins.
 */
void UConversationGraphDialogueNode::RestoreDynamicConnections(const TArray<UEdGraphPin*>& OldPins)
{
	// Build a mapping from old output pin name -> linked pins.
	TMap<FName, TArray<UEdGraphPin*>> OldLinksByName;

	for (UEdGraphPin* OldPin : OldPins)
	{
		if (!OldPin || OldPin->Direction != EGPD_Output)
		{
			continue;
		}

		if (OldPin->LinkedTo.Num() == 0)
		{
			continue;
		}

		TArray<UEdGraphPin*>& LinkedArray = OldLinksByName.FindOrAdd(OldPin->PinName);
		for (UEdGraphPin* Linked : OldPin->LinkedTo)
		{
			if (Linked)
			{
				LinkedArray.Add(Linked);
			}
		}
	}

	// Recreate connections on new pins with matching names.
	for (UEdGraphPin* NewPin : Pins)
	{
		if (!NewPin || NewPin->Direction != EGPD_Output)
		{
			continue;
		}

		if (TArray<UEdGraphPin*>* OldLinkedArray = OldLinksByName.Find(NewPin->PinName))
		{
			for (UEdGraphPin* OtherPin : *OldLinkedArray)
			{
				if (OtherPin)
				{
					NewPin->MakeLinkTo(OtherPin);
				}
			}
		}
	}
}

/**
 * Ensures pin names match the runtime dialogue choices.
 *
 * For each output pin:
 * - If a corresponding choice exists and has a non-empty ChoiceTitle, that
 *   title becomes the pin name.
 * - Otherwise, we fallback to "Choice_<Index>".
 */
void UConversationGraphDialogueNode::SyncPinNamesToChoices()
{
	UDialogueFlowDialogueNode* Runtime = GetDialogueNode();
	if (!Runtime)
	{
		return;
	}

	int32 OutputIdx = 0;

	for (UEdGraphPin* Pin : Pins)
	{
		if (!Pin || Pin->Direction != EGPD_Output)
		{
			continue;
		}

		if (Runtime->Choices.IsValidIndex(OutputIdx))
		{
			const FText& Title = Runtime->Choices[OutputIdx].ChoiceTitle;

			const FName NewName = Title.IsEmpty()
				? FName(*FString::Printf(TEXT("Choice_%d"), OutputIdx))
				: FName(*Title.ToString());

			Pin->PinName = NewName;
		}

		++OutputIdx;
	}
}

#if WITH_EDITOR
void UConversationGraphDialogueNode::HandleRuntimeNodePropertyChanged_Internal(const FPropertyChangedEvent& Event)
{
	HandleRuntimeNodePropertyChanged(); // call the safe version
}

void UConversationGraphDialogueNode::HandleRuntimeNodePropertyChanged()
{
	// Rebuild pins to match updated runtime data (Choices array changed)
	ReconstructNode();

	// Notify Slate graph so it redraws
	if (UEdGraph* Graph = GetGraph())
	{
		Graph->NotifyGraphChanged();
	}
}
#endif

#undef LOCTEXT_NAMESPACE
