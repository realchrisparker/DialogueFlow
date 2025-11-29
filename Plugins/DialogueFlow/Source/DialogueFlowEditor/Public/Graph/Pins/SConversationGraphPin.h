#pragma once

#include "CoreMinimal.h"
#include "SGraphPin.h"

class SConversationGraphPin : public SGraphPin
{
public:
    SLATE_BEGIN_ARGS(SConversationGraphPin) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
    {
        SGraphPin::Construct(SGraphPin::FArguments(), InPin);
    }

protected:
    virtual const FSlateBrush* GetPinIcon() const override
    {
        if (IsConnected())
        {
            return FAppStyle::Get().GetBrush("Graph.ExecPin.ConnectedHovered");
        }
        else
        {
            return FAppStyle::Get().GetBrush("Graph.ExecPin.DisconnectedHovered");
        }
    }
};
