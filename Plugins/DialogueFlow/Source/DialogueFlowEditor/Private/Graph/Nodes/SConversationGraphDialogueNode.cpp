// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: SConversationGraphDialogueNode.cpp
// Description: Slate widget implementation for Dialogue Nodes.
// ============================================================================

#include <Graph/Nodes/SConversationGraphDialogueNode.h>
#include <Graph/Nodes/ConversationGraphDialogueNode.h>
#include <Graph/Pins/SConversationGraphPinRound.h>
#include <Nodes/DialogueFlowDialogueNode.h>

#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Styling/AppStyle.h"

void SConversationGraphDialogueNode::Construct(
    const FArguments& InArgs,
    UConversationGraphDialogueNode* InNode)
{
    GraphNode = InNode;
    SetCursor(EMouseCursor::CardinalCross);
    UpdateGraphNode();
}

void SConversationGraphDialogueNode::UpdateGraphNode()
{
    InputPins.Empty();
    OutputPins.Empty();
    RightNodeBox.Reset();
    LeftNodeBox.Reset();

    GraphNode->AllocateDefaultPins();
    this->ContentScale.Bind(this, &SGraphNode::GetContentScale);

    // Cache runtime node
    UConversationGraphDialogueNode* GraphDialogue = Cast<UConversationGraphDialogueNode>(GraphNode);
    CachedDialogueNode = GraphDialogue ? GraphDialogue->GetDialogueNode() : nullptr;

    // Build main layout
    TSharedRef<SVerticalBox> Root =
        SNew(SVerticalBox)

        // TITLE BAR
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            BuildTitleBar()
        ]

        // Header (Speaker + Dialogue Text)
        + SVerticalBox::Slot()
        .AutoHeight()
        [
            BuildDialogueHeader(CachedDialogueNode)
        ]

        // Choices List
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(FMargin(0, 4))
        [
            BuildChoicesWidget(CachedDialogueNode)
        ]

        // Pin Layout (Input Left, Output Right)
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(FMargin(0, 6))
        [
            SNew(SHorizontalBox)

                + SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SAssignNew(LeftNodeBox, SVerticalBox)
                ]

                + SHorizontalBox::Slot()
                .FillWidth(1.f)
                .HAlign(HAlign_Right)
                [
                    SAssignNew(RightNodeBox, SVerticalBox)
                ]
        ];

    GetOrAddSlot(ENodeZone::Center)
        [
            SNew(SBorder)
                .BorderImage(FAppStyle::Get().GetBrush("Graph.Node.Body"))
                .Padding(FMargin(4))
                [
                    Root
                ]
        ];

    CreatePinWidgets();
}

// HEADER
TSharedRef<SWidget> SConversationGraphDialogueNode::BuildDialogueHeader(UDialogueFlowDialogueNode* DialogueNode)
{
    const FText Speaker =
        (DialogueNode && !DialogueNode->SpeakerName.IsEmpty())
        ? DialogueNode->SpeakerName
        : FText::FromString("Speaker");

    const FText Line =
        (DialogueNode && !DialogueNode->DialogueText.IsEmpty())
        ? DialogueNode->DialogueText
        : FText::FromString("New dialogue line...");

    return
        SNew(SBorder)
        .BorderImage(FAppStyle::Get().GetBrush("Graph.Node.TitleBackground"))
        .BorderBackgroundColor(FLinearColor(0.12f, 0.13f, 0.17f, 1.0f))
        .Padding(FMargin(8.f, 6.f))
        [
            SNew(SVerticalBox)

                // Speaker
                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    SNew(STextBlock)
                        .Text(Speaker)
                        .Font(FAppStyle::Get().GetFontStyle("BoldFont"))
                        .ColorAndOpacity(FLinearColor(0.85f, 0.87f, 1.f, 1.f))
                ]

                // Dialogue Text
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(FMargin(0.f, 4.f, 0.f, 0.f))
                [
                    SNew(STextBlock)
                        .Text(Line)
                        .WrapTextAt(280.f)
                        .ColorAndOpacity(FLinearColor(0.92f, 0.92f, 0.92f, 1.f))
                        .Font(FAppStyle::Get().GetFontStyle("NormalFont"))
                ]
        ];
}

// CHOICE LIST
TSharedRef<SWidget> SConversationGraphDialogueNode::BuildChoicesWidget(UDialogueFlowDialogueNode* DialogueNode)
{
    TSharedRef<SVerticalBox> List = SNew(SVerticalBox);

    if (!DialogueNode)
        return List;

    List->AddSlot()
        .AutoHeight()
        [
            SNew(STextBlock)
                .Text(FText::FromString("Choices"))
                .Font(FAppStyle::Get().GetFontStyle("BoldFont"))
                .ColorAndOpacity(FLinearColor(0.8f, 0.8f, 1.f))
        ];

    for (const FDialogueChoice& Choice : DialogueNode->Choices)
    {
        List->AddSlot()
            .AutoHeight()
            .Padding(FMargin(0, 2))
            [
                CreateChoiceEntry(Choice)
            ];
    }

    return List;
}

// CHOICE ENTRY (icons come from Choice, not DialogueNode)
TSharedRef<SWidget> SConversationGraphDialogueNode::CreateChoiceEntry(const FDialogueChoice& Choice)
{
    const FSlateBrush* Prefix =
        (Choice.PrefixIcon)
        ? new FSlateImageBrush(Choice.PrefixIcon, FVector2D(16, 16))
        : nullptr;

    const FSlateBrush* Suffix =
        (Choice.SuffixIcon)
        ? new FSlateImageBrush(Choice.SuffixIcon, FVector2D(16, 16))
        : nullptr;

    return
        SNew(SBorder)
        .BorderImage(FAppStyle::Get().GetBrush("Graph.Node.Body"))
        .Padding(FMargin(4.f, 2.f))
        .BorderBackgroundColor(FLinearColor(0.10f, 0.10f, 0.12f, 0.6f))
        [
            SNew(SHorizontalBox)

                // Prefix icon
                + SHorizontalBox::Slot()
                .AutoWidth()
                .VAlign(VAlign_Center)
                [
                    Prefix
                        ? StaticCastSharedRef<SWidget>(SNew(SImage).Image(Prefix))
                        : SNullWidget::NullWidget
                ]

                // Text
                + SHorizontalBox::Slot()
                .AutoWidth()
                .VAlign(VAlign_Center)
                .Padding(FMargin(6, 0))
                [
                    SNew(STextBlock)
                        .Text(Choice.ChoiceTitle.IsEmpty()
                            ? FText::FromString("Choice")
                            : Choice.ChoiceTitle)
                        .ColorAndOpacity(FLinearColor(0.96f, 0.96f, 0.96f))
                        .Font(FAppStyle::Get().GetFontStyle("NormalFont"))
                ]

            // Suffix icon
            + SHorizontalBox::Slot()
                .AutoWidth()
                .VAlign(VAlign_Center)
                .Padding(FMargin(6, 0, 0, 0))
                [
                    Suffix
                        ? StaticCastSharedRef<SWidget>(SNew(SImage).Image(Suffix))
                        : SNullWidget::NullWidget
                ]
        ];
}

// PIN WIDGETS
void SConversationGraphDialogueNode::CreatePinWidgets()
{
    InputPins.Empty();
    OutputPins.Empty();

    UEdGraphNode* Node = GraphNode;
    check(Node);

    for (UEdGraphPin* Pin : Node->Pins)
    {
        TSharedPtr<SGraphPin> NewPin = SNew(SConversationGraphPinRound, Pin);

        if (!NewPin.IsValid())
            continue;

        NewPin->SetOwner(SharedThis(this));

        if (Pin->Direction == EGPD_Input)
        {
            LeftNodeBox->AddSlot()
                .AutoHeight()
                [
                    NewPin.ToSharedRef()
                ];

            InputPins.Add(NewPin.ToSharedRef());
        }
        else
        {
            RightNodeBox->AddSlot()
                .AutoHeight()
                [
                    NewPin.ToSharedRef()
                ];

            OutputPins.Add(NewPin.ToSharedRef());
        }
    }
}

TSharedRef<SWidget> SConversationGraphDialogueNode::BuildTitleBar()
{
    // Try to load your dialogue icon from the editor style
    const FSlateBrush* IconBrush = FAppStyle::Get().GetBrush("GraphEditor.Comment_16x");

    return
        SNew(SBorder)
        .BorderImage(FAppStyle::Get().GetBrush("Graph.Node.TitleHighlight"))
        .Padding(FMargin(6.f, 4.f))
        .HAlign(HAlign_Left)
        .VAlign(VAlign_Center)
        [
            SNew(SHorizontalBox)

                // Icon
                + SHorizontalBox::Slot()
                .AutoWidth()
                .VAlign(VAlign_Center)
                [
                    SNew(SImage)
                        .Image(IconBrush)
                        .ColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.85f))
                ]

                // Title text
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(FMargin(6.f, 0.f, 0.f, 0.f))
                .VAlign(VAlign_Center)
                [
                    SNew(STextBlock)
                        .Text(FText::FromString(TEXT("Dialogue")))
                        .Font(FAppStyle::Get().GetFontStyle("BoldFont"))
                        .ColorAndOpacity(FLinearColor(0.95f, 0.95f, 1.f))
                ]
        ];
}
