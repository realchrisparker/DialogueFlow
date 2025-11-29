// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: DialogueFlowDialogueNode.h
// Description: Dialogue flow node that represents a single line of dialogue,
//              including speaker name, dialogue text, and simple playback options.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include <Nodes/DialogueFlowBaseNode.h>
#include <Structs/FDialogueChoice.h>
#include "DialogueFlowDialogueNode.generated.h"


// Forward declarations
class UDialogueFlowComponent;
class FDialogueFlowValidationContext;


/** Fired whenever a property on this dialogue node changes (editor only). */
#if WITH_EDITOR
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDialogueNodePropertyChanged, const FPropertyChangedEvent&);
#endif


/**
 * UDialogueFlowDialogueNode
 *
 * Runtime dialogue node including:
 * - NPC speaker name
 * - NPC spoken line (DialogueText)
 * - Zero or more player-selectable choices
 *
 * This matches AAA dialogue structure (Cyberpunk / Mass Effect / Witcher):
 *  NPC speaks → Player sees clickable choices → Selected output branch executes
 */
UCLASS(BlueprintType, EditInlineNew)
class DIALOGUEFLOW_API UDialogueFlowDialogueNode : public UDialogueFlowBaseNode
{
    GENERATED_BODY()

public:

    /*
     * Functions
    */

    /** Default constructor. Initializes default display name and node color. */
    UDialogueFlowDialogueNode();

    /**
     * Returns the node category label used in debugging and editor UI.
     * For Dialogue nodes this returns the string "Dialogue".
     */
    virtual FString GetNodeCategory() const override { return TEXT("Dialogue"); }

    /**
     * Returns the node type as Dialogue.
     * Used by the runtime system to quickly identify node behavior.
     */
    virtual EDialogueFlowNodeType GetNodeType() const override { return EDialogueFlowNodeType::Dialogue; }

    /**
     * Runtime execution entry point for a Dialogue node.
     *
     * Current behavior:
     * - Does not perform any side effects.
     * - Immediately calls ExecuteNext(RuntimeComponent) to continue the flow.
     *
     * Future behavior (once UDialogueFlowComponent is implemented):
     * - Notify the runtime component that a dialogue line has become active.
     * - Allow the component to pause and present the line to the player.
     * - Resume flow when the component decides to advance.
     *
     * @param RuntimeComponent  The component evaluating this dialogue flow.
     */
    virtual void OnExecuteNode(UDialogueFlowComponent* RuntimeComponent) override;

    /**
     * Validates Dialogue node data without graph context.
     *
     * Checks:
     * - DialogueText must not be empty.
     *
     * @param OutErrorMessage  Receives a human-readable error message on failure.
     * @return true if the node's configuration is valid; false otherwise.
     */
    virtual bool IsNodeValid(FString& OutErrorMessage) const override;

    /*
    * Properties
    */

    /**
     * Optional speaker display name shown to the player.
     *
     * Example: "NPC Guard", "Hero", or a localized, in-world character name.
     * This is purely presentational and does not enforce any runtime lookup.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (DisplayName = "Speaker Name"))
    FText SpeakerName;

    /**
     * Localized dialogue text presented to the player.
     *
     * This is the primary content of the node and should almost always be set.
     * The text supports multiple lines in the editor.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (DisplayName = "Dialogue Text", MultiLine = "true"))
    FText DialogueText;

    /**
     * Optional voice-over audio played when this dialogue line is shown.
     *
     * This may be a SoundWave, SoundCue, MetaSound, or any USoundBase-derived asset.
     * If null, no audio is played and the line is text-only.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (DisplayName = "Voice Audio"))
    TObjectPtr<USoundBase> VoiceAudio = nullptr;

    /**
     * Choices presented to the player after this dialogue line.
     *
     * Each choice represents a possible response or action the player can take,
     * leading to different subsequent nodes in the dialogue flow.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (DisplayName = "Dialogue Choices"))
    TArray<FDialogueChoice> Choices;

    /**
     * If true, the runtime system is allowed to auto-advance from this node
     * after AutoAdvanceDelay seconds, instead of waiting for explicit player input.
     *
     * This flag is a hint for UDialogueFlowComponent and does not enforce behavior
     * on its own.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (DisplayName = "Auto Advance"))
    bool bAutoAdvance = false;

    /**
     * Time in seconds before automatically advancing to the next node when
     * bAutoAdvance is true.
     *
     * A value of 0.0 means "advance immediately" once the line is shown.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue",
        meta = (DisplayName = "Auto Advance Delay", EditCondition = "bAutoAdvance", ClampMin = "0.0"))
    float AutoAdvanceDelay = 0.0f;

#if WITH_EDITOR
    FOnDialogueNodePropertyChanged& OnPropertyChangedEvent() { return PropertyChangedDelegate; }
#endif

#if WITH_EDITOR
    /*
     * Functions
    */

    /**
     * Editor-only: returns the body color used when rendering this node.
     *
     * Uses the NodeColor property so designers can tweak the default tint
     * if desired, while still providing a sensible blue-toned default.
     *
     * @return Linear color used to tint the node body in the graph.
     */
    virtual FLinearColor GetNodeBodyColor() const override;

    /**
     * Editor-only: returns a concise description for use in the node body.
     *
     * Format:
     * - If SpeakerName is set: "SpeakerName: DialogueText"
     * - Otherwise: "DialogueText"
     *
     * The DialogueText is not truncated here; any truncation is handled by
     * the editor widget displaying the text.
     *
     * @return Localized description summarizing this dialogue line.
     */
    virtual FText GetNodeDescription() const override;

    /**
     * Editor-only: validates this node within the context of the entire graph.
     *
     * Additional rules can be added later (e.g., ensuring at least one output,
     * or forbidding Dialogue nodes from having more than N outputs).
     *
     * @param Context  Validation context providing graph-level information.
     */
    virtual void ValidateNode(FDialogueFlowValidationContext& Context) const override;

    /**
     * Editor-only: called when a property is changed in the editor.
     * Used to notify the containing graph of changes.
     * @param PropertyChangedEvent  Information about the property change.
     */
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    /*
     * Properties
    */

    FOnDialogueNodePropertyChanged PropertyChangedDelegate;

#endif // WITH_EDITOR
};
