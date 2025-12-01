// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationGraphSchema.h
// Description: Schema controlling rules for pin linking, node creation,
//              and context-menu actions for Dialogue Flow Editor.
// ============================================================================

#include "Graph/ConversationGraphSchema.h"
#include "Graph/Nodes/ConversationGraphNode.h"
#include "Graph/Nodes/ConversationGraphStartNode.h"
#include "Graph/Nodes/ConversationGraphEndNode.h"
#include "Graph/Nodes/ConversationGraphDialogueNode.h"
#include "Framework/Commands/GenericCommands.h"
#include "GraphEditorActions.h"   // Required for FGraphEditorCommands
#include "EdGraph/EdGraphPin.h"


#define LOCTEXT_NAMESPACE "ConversationGraphSchema"


void UConversationGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	// CATEGORY: Dialogue
	const FText Category = NSLOCTEXT("DialogueFlow", "DialogueNodes", "Dialogue");

	// Add Dialogue Node
	{
		TSharedPtr<FEdGraphSchemaAction> Action = MakeShareable(
			new FEdGraphSchemaAction_NewNode(
				Category,
				NSLOCTEXT("DialogueFlow", "AddDialogueNode", "Dialogue Node"),
				NSLOCTEXT("DialogueFlow", "AddDialogueNodeTooltip", "Adds a new Dialogue node."),
				0));

		UConversationGraphDialogueNode* Template = NewObject<UConversationGraphDialogueNode>(ContextMenuBuilder.OwnerOfTemporaries);
		((FEdGraphSchemaAction_NewNode*)Action.Get())->NodeTemplate = Template;

		ContextMenuBuilder.AddAction(Action);
	}

	// Add End Node
	// {
	// 	TSharedPtr<FEdGraphSchemaAction> Action = MakeShareable(
	// 		new FEdGraphSchemaAction_NewNode(
	// 			Category,
	// 			NSLOCTEXT("DialogueFlow", "AddEndNode", "End Node"),
	// 			NSLOCTEXT("DialogueFlow", "AddEndNodeTooltip", "Adds a new End node."),
	// 			1));

	// 	UConversationGraphEndNode* Template = NewObject<UConversationGraphEndNode>(ContextMenuBuilder.OwnerOfTemporaries);
	// 	((FEdGraphSchemaAction_NewNode*)Action.Get())->NodeTemplate = Template;

	// 	ContextMenuBuilder.AddAction(Action);
	// }
}

const FPinConnectionResponse UConversationGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (A == B)
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot connect pin to itself."));

	if (A->Direction == B->Direction)
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Pins must be opposite directions."));

	UConversationGraphNode* NodeA = Cast<UConversationGraphNode>(A->GetOwningNode());
	UConversationGraphNode* NodeB = Cast<UConversationGraphNode>(B->GetOwningNode());

	if (!NodeA || !NodeB)
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Invalid node types."));

	// Start node cannot have inputs
	if ((NodeA->IsA(UConversationGraphStartNode::StaticClass()) && A->Direction == EGPD_Input) ||
		(NodeB->IsA(UConversationGraphStartNode::StaticClass()) && B->Direction == EGPD_Input))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Start node cannot have inputs."));
	}

	// End node cannot have outputs
	if ((NodeA->IsA(UConversationGraphEndNode::StaticClass()) && A->Direction == EGPD_Output) ||
		(NodeB->IsA(UConversationGraphEndNode::StaticClass()) && B->Direction == EGPD_Output))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("End node cannot have outputs."));
	}

	// Everything else allowed
	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

bool UConversationGraphSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	const FPinConnectionResponse Resp = CanCreateConnection(A, B);

	if (Resp.Response == CONNECT_RESPONSE_MAKE)
	{
		A->MakeLinkTo(B);
		return true;
	}

	return false;
}

void UConversationGraphSchema::GetContextMenuActions(
	UToolMenu* Menu,
	UGraphNodeContextMenuContext* Context) const
{
	if (!Menu || !Context)
	{
		return;
	}

	// Node actions (Delete)
	if (const UEdGraphNode* Node = Context->Node)
	{
		FToolMenuSection& Section = Menu->AddSection("ConversationNodeActions", LOCTEXT("NodeActions", "Node"));

		// CanUserDeleteNode() is const, so this works fine with const UEdGraphNode*
		if (Node->CanUserDeleteNode())
		{
			Section.AddMenuEntry(FGenericCommands::Get().Delete);
		}
	}

	// -----------------------------------------------------
	// Pin actions (Break Links)
	// -----------------------------------------------------
	if (const UEdGraphPin* Pin = Context->Pin)
	{
		if (Pin->LinkedTo.Num() > 0)
		{
			FToolMenuSection& Section = Menu->AddSection("ConversationPinActions", LOCTEXT("PinActions", "Pin"));
			Section.AddMenuEntry(FGraphEditorCommands::Get().BreakPinLinks);
		}
	}

	// Background actions (Add Dialogue Node)
	if (!Context->Node && !Context->Pin)
	{
		FGraphContextMenuBuilder Builder(Context->Graph);
		GetGraphContextActions(Builder); // Your existing Dialogue/End actions

		// Pull actions into the ToolMenu
		FToolMenuSection& Section = Menu->AddSection("ConversationAddNode", LOCTEXT("AddNode", "Add Node"));

		const int32 NumActions = Builder.GetNumActions();
		for (int32 i = 0; i < NumActions; ++i)
		{
			TSharedPtr<FEdGraphSchemaAction> Action = Builder.GetSchemaAction(i);
			if (!Action.IsValid())
				continue;

			FText MenuDesc = Action->GetMenuDescription();
			FText Tooltip = Action->GetTooltipDescription();

			Section.AddMenuEntry(
				NAME_None,
				MenuDesc,
				Tooltip,
				FSlateIcon(),
				FUIAction(FExecuteAction::CreateLambda(
					[ Action, Context ] ()
					{
						const UEdGraph* ConstGraph = Context->Graph.Get();
						UEdGraph* MutableGraph = const_cast<UEdGraph*>(ConstGraph);

						// UE 5.6 does NOT provide click location in UGraphNodeContextMenuContext
						const FVector2f SpawnPos(0.f, 0.f);

						Action->PerformAction(
							MutableGraph,
							nullptr,
							SpawnPos,
							true
						);
					}))
			);
		}
	}
}

#undef LOCTEXT_NAMESPACE
