#include <Graph/Nodes/SConversationGraphEndNode.h>
#include <Graph/Pins/SConversationGraphPin.h>
#include "SlateOptMacros.h"
#include "GraphEditorSettings.h"
#include "SGraphPin.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBorder.h"

void SConversationGraphEndNode::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
    this->GraphNode = InNode;
    SetCursor(EMouseCursor::GrabHand);

    UpdateGraphNode();
}

void SConversationGraphEndNode::UpdateGraphNode()
{
    // Clear
    InputPins.Empty();
    OutputPins.Empty();
    RightNodeBox.Reset();
    LeftNodeBox.Reset();

    GraphNode->AllocateDefaultPins();

    this->ContentScale.Bind(this, &SGraphNode::GetContentScale);

    //
    // ────────────────────────────────────────────────────────────────
    //   MAIN NODE BODY (Mirrors Start Node)
    // ────────────────────────────────────────────────────────────────
    //
    TSharedRef<SWidget> NodeBody =
        SNew(SBorder)
        .BorderImage(GetEntryBackgroundBrush())
        .Padding(FMargin(12.f, 4.f))
        [
            SNew(SHorizontalBox)

                // LEFT PIN CONTAINER (End node has INPUT)
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(0.f, 0.f)
                [
                    SAssignNew(LeftNodeBox, SVerticalBox)
                ]

                // Title
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(6.f, 0.f)
                [
                    SNew(STextBlock)
                        .Text(FText::FromString("End Conversation"))
                        .TextStyle(FAppStyle::Get(), "Graph.StateNode.NodeTitle")
                ]
        ];

    this->GetOrAddSlot(ENodeZone::Center)
        [
            NodeBody
        ];

    //
    // ────────────────────────────────────────────────────────────────
    //   CREATE ONLY ONE INPUT PIN (End Node Behavior)
    // ────────────────────────────────────────────────────────────────
    //
    UEdGraphPin* InputPin = nullptr;

    for (UEdGraphPin* Pin : GraphNode->Pins)
    {
        if (Pin->Direction == EGPD_Input)
        {
            InputPin = Pin;
            break;
        }
    }

    if (InputPin)
    {
        // TSharedPtr<SGraphPin> PinWidget = CreatePinWidget(InputPin);
        TSharedPtr<SGraphPin> PinWidget = SNew(SConversationGraphPin, InputPin);
        if (PinWidget.IsValid())
        {
            PinWidget->SetOwner(SharedThis(this));
            PinWidget->SetShowLabel(false);

            LeftNodeBox->AddSlot()
                .AutoHeight()
                [
                    PinWidget.ToSharedRef()
                ];

            InputPins.Add(PinWidget.ToSharedRef());
        }
    }
}

void SConversationGraphEndNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
    PinToAdd->SetOwner(SharedThis(this));

    LeftNodeBox->AddSlot()
        .AutoHeight()
        [
            PinToAdd
        ];
}

void SConversationGraphEndNode::CreatePinWidgets()
{
    // End node uses custom layout just like the Start node.
}

const FSlateBrush* SConversationGraphEndNode::GetEntryBackgroundBrush() const
{
    return FAppStyle::Get().GetBrush("Graph.StateNode.Body");
}
