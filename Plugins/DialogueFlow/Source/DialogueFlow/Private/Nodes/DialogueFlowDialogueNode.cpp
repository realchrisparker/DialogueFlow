// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: DialogueFlowDialogueNode.cpp
// Description: Implementation of the dialogue flow node used to display a line
//              of dialogue and a set of selectable choices.
// ============================================================================

#include <Nodes/DialogueFlowDialogueNode.h>


#define LOCTEXT_NAMESPACE "DialogueFlowDialogueNode"

UDialogueFlowDialogueNode::UDialogueFlowDialogueNode()
{
    NodeDisplayName = LOCTEXT("DialogueNodeName", "Dialogue");
    NodeColor = FLinearColor(0.20f, 0.25f, 0.80f); // Soft blue/purple tone
}

void UDialogueFlowDialogueNode::OnExecuteNode(UDialogueFlowComponent* RuntimeComponent)
{
    if (!RuntimeComponent)
    {
        return;
    }

    // RuntimeComponent will:
    // - Display SpeakerName + DialogueText
    // - Display Choices[] in UI
    // - Wait for player selection (unless auto-advance)
    // - Call ExecuteOutput(this, SelectedPinIndex)
    // RuntimeComponent->HandleDialogueNode(this);
}

bool UDialogueFlowDialogueNode::IsNodeValid(FString& OutErrorMessage) const
{
    if (DialogueText.IsEmpty())
    {
        OutErrorMessage = TEXT("Dialogue node is missing Dialogue Text.");
        return false;
    }

    // Validate choice text
    for (int32 i = 0; i < Choices.Num(); i++)
    {
        if (Choices[i].ChoiceTitle.IsEmpty())
        {
            OutErrorMessage = FString::Printf(
                TEXT("Choice %d is missing a Choice Title."), i);
            return false;
        }
    }

    return true;
}

#if WITH_EDITOR

FLinearColor UDialogueFlowDialogueNode::GetNodeBodyColor() const
{
    return NodeColor;
}

FText UDialogueFlowDialogueNode::GetNodeDescription() const
{
    if (!SpeakerName.IsEmpty())
    {
        return FText::Format(
            LOCTEXT("DialogueDescWithSpeaker", "{0}: {1}"),
            SpeakerName,
            DialogueText
        );
    }

    return DialogueText;
}

void UDialogueFlowDialogueNode::ValidateNode(FDialogueFlowValidationContext& Context) const
{
    FString Error;
    if (!IsNodeValid(Error))
    {
        // The validation context will handle reporting in the editor UI.
        // Example (once context API is defined):
        // Context.ReportNodeError(this, FText::FromString(Error));
    }

    // Future:
    // - Warn if node has no choices and bAutoAdvance=false (dead end)
    // - Warn if node has choices but no output pins
}

void UDialogueFlowDialogueNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // Broadcast to listeners in the editor module
    PropertyChangedDelegate.Broadcast(PropertyChangedEvent);
}

#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE
