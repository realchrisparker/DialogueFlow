#include <Nodes/DialogueFlowStartNode.h>
// #include "DialogueFlowComponent.h"


UDialogueFlowStartNode::UDialogueFlowStartNode()
{
    NodeDisplayName = FText::FromString("Start");
    NodeColor = FLinearColor(0.2f, 0.8f, 0.4f);
}

void UDialogueFlowStartNode::OnExecuteNode(UDialogueFlowComponent* RuntimeComponent)
{
    // TODO: Implement when DialogueFlowComponent is created.
    ExecuteNext(RuntimeComponent);
}

#if WITH_EDITOR
void UDialogueFlowStartNode::ValidateNode(FDialogueFlowValidationContext& Context) const
{
    // Validation system not implemented yet.
}
#endif
