#include <Nodes/DialogueFlowBaseNode.h>
// #include "Components/DialogueFlowComponent.h"


/** Base constructor. */
UDialogueFlowBaseNode::UDialogueFlowBaseNode()
{
    NodeTitle = FText::FromString("Node");
}

void UDialogueFlowBaseNode::ExecuteNext(UDialogueFlowComponent* RuntimeComponent)
{
    // Component does not exist yet — do nothing.
}
