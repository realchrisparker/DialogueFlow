#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class SConversationGraphEndNode : public SGraphNode
{
public:
    SLATE_BEGIN_ARGS(SConversationGraphEndNode) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UEdGraphNode* InNode);

    virtual void UpdateGraphNode() override;
    virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
    virtual void CreatePinWidgets() override;

protected:
    const FSlateBrush* GetEntryBackgroundBrush() const;
};
