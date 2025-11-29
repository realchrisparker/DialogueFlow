// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: SConversationGraphPinRound.h
// Description: Custom round Slate graph pin used for Dialogue nodes.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "SGraphPin.h"

/**
 * SConversationGraphPinRound
 *
 * A custom SGraphPin that draws a round white “flow style” pin.
 * This matches the pin style used in Behavior Trees, State Trees,
 * and other UE frameworks.
 */
class SConversationGraphPinRound : public SGraphPin
{
public:

    SLATE_BEGIN_ARGS(SConversationGraphPinRound) {}
    SLATE_END_ARGS()

    /** Constructs the pin widget using the supplied graph pin object. */
    void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
    {
        this->SetCursor(EMouseCursor::CardinalCross);

        // Build the standard SGraphPin
        SGraphPin::Construct(SGraphPin::FArguments(), InPin);

        // Correct way to hide labels (ShouldShowLabel does not exist)
        this->SetShowLabel(false);

        // Build a forced-white circular pin widget
        TSharedRef<SWidget> PinWidget =
            SNew(SBox)
            .WidthOverride(18.f)
            .HeightOverride(18.f)
            [
                SNew(SImage)
                    .Image(GetPinIcon())                    // Keep the default circular brush
                    .ColorAndOpacity(FLinearColor::White)   // FORCE WHITE
            ];

        // Install the custom appearance
        SetPinImageWidget(PinWidget);
    }

protected:

    /** Returns the circular pin brush. */
    virtual const FSlateBrush* GetPinIcon() const override
    {
        // BehaviorTree-style white circular pins
        return IsConnected()
            ? FAppStyle::Get().GetBrush("Graph.Pin.Connected")
            : FAppStyle::Get().GetBrush("Graph.Pin.Disconnected");
    }

    /** Force the pin tint to pure white instead of the schema’s type color. */
    virtual FSlateColor GetPinColor() const override
    {
        return FSlateColor(FLinearColor::White);
    }
};
