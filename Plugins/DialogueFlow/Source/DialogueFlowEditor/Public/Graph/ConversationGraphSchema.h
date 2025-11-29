// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationGraphSchema.h
// Description: Graph schema defining rules and behavior for dialogue graphs.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "ConversationGraphSchema.generated.h"

/**
 * Graph schema that enforces strict connection rules and provides context menu
 * actions for Dialogue Flow graphs.
 */
UCLASS()
class DIALOGUEFLOWEDITOR_API UConversationGraphSchema : public UEdGraphSchema
{
    GENERATED_BODY()

public:

    virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
    virtual void GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
    virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;    
    virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;

protected:

    /** Adds Start and End node creation items to the context menu. */
    void AddNodeActions(FGraphContextMenuBuilder& ContextMenuBuilder) const;
};
