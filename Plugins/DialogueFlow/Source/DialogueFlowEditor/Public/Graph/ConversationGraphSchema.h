// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationGraphSchema.h
// Description: Schema controlling rules for pin linking, node creation,
//              and context-menu actions for Dialogue Flow Editor.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "ConversationGraphSchema.generated.h"

/**
 * Schema defining all rules for:
 * - Context menu actions
 * - Node creation
 * - Connection rules
 * - Logical link validation
 */
UCLASS()
class DIALOGUEFLOWEDITOR_API UConversationGraphSchema : public UEdGraphSchema
{
    GENERATED_BODY()

public:

    /** Builds right-click context actions (Add Start, End, Dialogue nodes). */
    virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;

    /** Defines whether two pins are allowed to connect. */
    virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;

    /** Performs the connection if allowed. */
    virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;

    /** Pure cleanup — does NOT break serialization. */
    virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodesEvents) const override
    {
        Super::BreakPinLinks(TargetPin, bSendsNodesEvents);
    }

    /** Pure cleanup — does NOT break serialization. */
    virtual void BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const override
    {
        Super::BreakSinglePinLink(SourcePin, TargetPin);
    }

    /** Required for deletion. */
    virtual void BreakNodeLinks(UEdGraphNode& TargetNode) const override
    {
        TargetNode.BreakAllNodeLinks();
    }

    /** Graph type classification. */
    virtual EGraphType GetGraphType(const UEdGraph* TestEdGraph) const override
    {
        return EGraphType::GT_Ubergraph;
    }

    virtual void GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
};
