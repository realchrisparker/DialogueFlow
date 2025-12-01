// ============================================================================
// Copyright…
#include "Graph/Nodes/ConversationGraphNode.h"
#include "Nodes/DialogueFlowBaseNode.h"
#include "EdGraph/EdGraphPin.h"


UConversationGraphNode::UConversationGraphNode(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    CreateNewGuid(); // very important
}

void UConversationGraphNode::AllocateDefaultPins()
{
    Pins.Reset();

    // Base nodes have one input and one output pin by default.
    CreatePin(EGPD_Input, TEXT("DialogueFlow"), FName("In"));
    CreatePin(EGPD_Output, TEXT("DialogueFlow"), FName("Out"));
}

void UConversationGraphNode::ReconstructNode()
{
    // Save old pins before rebuild
    TArray<UEdGraphPin*> OldPins = Pins;

    AllocateDefaultPins();

    // Map old → new pins and restore persistent data
    for (const FConversationSavedPinLink& Saved : SavedConnectionData.SavedLinks)
    {
        UEdGraphPin* NewThisPin = nullptr;
        UEdGraphPin* NewOtherPin = nullptr;

        for (UEdGraphPin* Pin : Pins)
        {
            if (Pin->PersistentGuid == Saved.ThisPinId)
                NewThisPin = Pin;
            if (Pin->PersistentGuid == Saved.LinkedPinId)
                NewOtherPin = Pin;
        }

        if (NewThisPin && NewOtherPin)
            NewThisPin->MakeLinkTo(NewOtherPin);
    }
}

void UConversationGraphNode::Serialize(FArchive& Ar)
{
    Super::Serialize(Ar);

    if (Ar.IsSaving())
    {
        SavedConnectionData.SavedLinks.Empty();

        for (UEdGraphPin* Pin : Pins)
        {
            if (!Pin) continue;

            for (UEdGraphPin* Linked : Pin->LinkedTo)
            {
                if (!Linked) continue;

                FConversationSavedPinLink Saved;
                Saved.ThisPinId = Pin->PersistentGuid;
                Saved.LinkedPinId = Linked->PersistentGuid;

                SavedConnectionData.SavedLinks.Add(Saved);
            }
        }
    }
}

void UConversationGraphNode::PostLoad()
{
    Super::PostLoad();

    // Delay restore until pins rebuilt by UE
    FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(FSimpleDelegateGraphTask::FDelegate::CreateLambda([ this ] ()
        {
            RestoreConnections();
        }), TStatId(), nullptr, ENamedThreads::GameThread);
}

void UConversationGraphNode::RestoreConnections()
{
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
            ThisPin->MakeLinkTo(OtherPin);
    }
}

UDialogueFlowBaseNode* UConversationGraphNode::GetNodeData() const
{
    return Cast<UDialogueFlowBaseNode>(GetGraph()->GetOuter());
}

void UConversationGraphNode::PostEditUndo()
{
    Super::PostEditUndo();

    // Rebuild pins and restore serialized pin links
    ReconstructNode();

    // Notify owning graph so SGraphEditor updates
    if (UEdGraph* Graph = GetGraph())
    {
        Graph->NotifyGraphChanged();
    }
}
