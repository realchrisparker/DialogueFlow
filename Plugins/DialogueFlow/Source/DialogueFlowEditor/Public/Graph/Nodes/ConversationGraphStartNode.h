#pragma once

#include "CoreMinimal.h"
#include <Graph/Nodes/ConversationGraphNode.h>
#include "ConversationGraphStartNode.generated.h"

UCLASS()
class DIALOGUEFLOWEDITOR_API UConversationGraphStartNode : public UConversationGraphNode
{
    GENERATED_BODY()

public:
    UConversationGraphStartNode(const FObjectInitializer& ObjectInitializer);

    virtual void AllocateDefaultPins() override;

    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
    
    virtual FLinearColor GetNodeTitleColor() const override;

    virtual FText GetTooltipText() const override;

    virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;

    virtual bool CanDuplicateNode() const override { return false; }

    virtual bool CanUserDeleteNode() const override { return false; }

    /** Custom Slate widget */
    virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
};
