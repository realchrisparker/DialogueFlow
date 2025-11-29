#include <Graph/Nodes/SConversationGraphStartNode.h>
#include <Graph/Pins/SConversationGraphPin.h>
#include "SlateOptMacros.h"
#include "GraphEditorSettings.h"
#include "SGraphPin.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBorder.h"

void SConversationGraphStartNode::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
    this->GraphNode = InNode;
    SetCursor(EMouseCursor::GrabHand);

    UpdateGraphNode();
}

void SConversationGraphStartNode::UpdateGraphNode()
{
    // Clear old content
    InputPins.Empty();
    OutputPins.Empty();
    RightNodeBox.Reset();
    LeftNodeBox.Reset();

    // Let the underlying UEdGraphNode create its pins
    GraphNode->AllocateDefaultPins();

    this->ContentScale.Bind(this, &SGraphNode::GetContentScale);

    //
    // ────────────────────────────────────────────────────────────────
    //   MAIN NODE BODY (Entry Node Style)
    // ────────────────────────────────────────────────────────────────
    //
    TSharedRef<SWidget> NodeBody =
        SNew(SBorder)
        .BorderImage(GetEntryBackgroundBrush())
        .Padding(FMargin(12.f, 4.f))
        [
            SNew(SHorizontalBox)

                // Title: "Start Conversation"
                + SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(STextBlock)
                        .Text(FText::FromString("Start Conversation"))
                        .TextStyle(FAppStyle::Get(), "Graph.StateNode.NodeTitle")
                ]

                // Pin container (right side)
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(6.f, 0.f)
                [
                    SAssignNew(RightNodeBox, SVerticalBox)
                ]
        ];

    // Install node body into the center area
    this->GetOrAddSlot(ENodeZone::Center)
        [
            NodeBody
        ];

    //
    // ────────────────────────────────────────────────────────────────
    //   CREATE ONLY ONE OUTPUT PIN (Start Node Expected Behavior)
    // ────────────────────────────────────────────────────────────────
    //
    UEdGraphPin* OutputPin = nullptr;

    for (UEdGraphPin* Pin : GraphNode->Pins)
    {
        if (Pin->Direction == EGPD_Output)
        {
            OutputPin = Pin;
            break; // Only one output for start node
        }
    }

    if (OutputPin)
    {
        // TSharedPtr<SGraphPin> PinWidget = CreatePinWidget(OutputPin);
        TSharedPtr<SGraphPin> PinWidget = SNew(SConversationGraphPin, OutputPin);
        if (PinWidget.IsValid())
        {
            //
            // ❗ FIX: THIS WAS MISSING AND CAUSED THE CRASH
            //
            PinWidget->SetOwner(SharedThis(this));   // REQUIRED FIX ✔✔✔

            PinWidget->SetShowLabel(false);

            RightNodeBox->AddSlot()
                .AutoHeight()
                [
                    PinWidget.ToSharedRef()
                ];

            OutputPins.Add(PinWidget.ToSharedRef());
        }
    }

    // Do NOT call CreatePinWidgets(); you're manually controlling pin creation.
}

void SConversationGraphStartNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
    // Correct behavior for any externally added pins
    PinToAdd->SetOwner(SharedThis(this));

    RightNodeBox->AddSlot()
        .AutoHeight()
        [
            PinToAdd
        ];
}

const FSlateBrush* SConversationGraphStartNode::GetEntryBackgroundBrush() const
{
    // Use the same background brush as the Animation State Machine entry node
    return FAppStyle::Get().GetBrush("Graph.StateNode.Body");
}

void SConversationGraphStartNode::CreatePinWidgets()
{
    // Intentionally not creating default K2 pins
    // The start node uses custom layout in UpdateGraphNode()
}
