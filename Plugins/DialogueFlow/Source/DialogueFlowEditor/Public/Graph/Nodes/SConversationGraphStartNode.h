#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class SConversationGraphStartNode : public SGraphNode
{
public:
    SLATE_BEGIN_ARGS(SConversationGraphStartNode) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UEdGraphNode* InNode);

    // Override these to remove title and customize look
    virtual void UpdateGraphNode() override;
    virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
    virtual void CreatePinWidgets() override;

protected:
    const FSlateBrush* GetEntryBackgroundBrush() const;
};
