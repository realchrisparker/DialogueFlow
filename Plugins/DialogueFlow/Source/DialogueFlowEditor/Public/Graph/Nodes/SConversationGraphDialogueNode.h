// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: SConversationGraphDialogueNode.h
// Description: Slate widget that visually represents a dialogue node,
//              including dialogue text, speaker, and multiple choices.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include <Structs/FDialogueChoice.h>

class UConversationGraphDialogueNode;
class UDialogueFlowDialogueNode;

/**
 * SConversationGraphDialogueNode
 *
 * Visual Slate widget representing a Dialogue Node in the editor graph.
 *
 * UI Regions:
 * - Title area (Speaker Name)
 * - Dialogue text bubble
 * - Choice list (with Prefix/Suffix icons)
 * - Input pin (left)
 * - Dynamic output pins (right)
 */
class SConversationGraphDialogueNode : public SGraphNode
{
public:

    SLATE_BEGIN_ARGS(SConversationGraphDialogueNode) {}
        SLATE_ARGUMENT(UConversationGraphDialogueNode*, GraphNodeObj)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UConversationGraphDialogueNode* InNode);

    /** Rebuilds the entire visual layout of the node. */
    virtual void UpdateGraphNode() override;

    /** Creates pin widgets for input/output pins. */
    virtual void CreatePinWidgets() override;

protected:

    /** Cached pointer to the runtime DialogueFlow node (may be null during creation). */
    UDialogueFlowDialogueNode* CachedDialogueNode = nullptr;

    /** Builds the speaker + dialogue text region. */
    TSharedRef<SWidget> BuildDialogueHeader(UDialogueFlowDialogueNode* DialogueNode);

    /** Builds the choice list UI for Choices[]. */
    TSharedRef<SWidget> BuildChoicesWidget(UDialogueFlowDialogueNode* DialogueNode);

    /** Creates a single choice entry with icons and text. */
    TSharedRef<SWidget> CreateChoiceEntry(const FDialogueChoice& Choice);

    /** Title bar (icon + "Dialogue") */
    TSharedRef<SWidget> BuildTitleBar();
};
