#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include <Enums/DialogueFlowNodeTypes.h>
#include "DialogueFlowBaseNode.generated.h"

/**
 * Base class for dialogue flow nodes.
 *
 * This class represents a generic node inside a DialogueFlow asset.
 * All node types (Start, End, Dialogue, Event, Condition) inherit from this.
 *
 * Responsibilities:
 * - Store unique node ID and designer-visible title
 * - Store graph/editor metadata (position, connections)
 * - Provide virtual functions for editor visualization, validation,
 *   and runtime execution (overridden by subclasses)
 *
 * This class is purely data + behavior. Graph visualization and UEdGraphNode
 * wrappers will be implemented separately in the editor module.
 */
UCLASS(Abstract, BlueprintType, EditInlineNew)
class DIALOGUEFLOW_API UDialogueFlowBaseNode : public UObject
{
    GENERATED_BODY()

public:

    /*
     * Functions
    */

    /** Base constructor. */
    UDialogueFlowBaseNode();

    /**
     * Returns a simple string identifying the node’s category.
     *
     * Useful for internal debugging and editor UI.
     * Subclasses override to return values such as "Start", "Dialogue", etc.
     */
    virtual FString GetNodeCategory() const { return TEXT("Base"); }

    /**
     * Returns the node type as an enum value.
     *
     * Subclasses must override to supply their type (Start, End, Dialogue, etc.).
     * This provides a fast lookup for runtime node evaluation.
     */
    UFUNCTION(BlueprintPure)
    virtual EDialogueFlowNodeType GetNodeType() const { return EDialogueFlowNodeType::Unknown; }

    /**
     * Runtime execution entry point.
     *
     * Called by the DialogueFlowComponent when this node becomes active.
     * Subclasses implement logic such as:
     *  - Display text (DialogueNode)
     *  - Fire an event (EventNode)
     *  - Evaluate logic (ConditionNode)
     *  - Transition to next node(s)
     *
     * @param RuntimeComponent  The component evaluating this dialogue flow.
     */
    virtual void OnExecuteNode(class UDialogueFlowComponent* RuntimeComponent) {}

    /**
     * Helper to forward execution to the next connected node(s).
     *
     * Typically called at the end of OnExecuteNode implementations.
     *
     * @param RuntimeComponent  The component evaluating this dialogue flow.
     */
    virtual void ExecuteNext(UDialogueFlowComponent* RuntimeComponent);

    /**
     * Validates the node’s data.
     *
     * Used by the asset when saving or compiling the dialogue graph.
     * Subclasses override to supply field-specific validation rules.
     *
     * @param OutErrorMessage  Receives the error text if validation fails.
     * @return true if valid; false if this node contains configuration errors.
     */
    virtual bool IsNodeValid(FString& OutErrorMessage) const { return true; }

#if WITH_EDITOR
    /**
     * Editor-only: returns a color used when rendering the node body.
     *
     * Subclasses override to differentiate their visual appearance.
     */
    virtual FLinearColor GetNodeBodyColor() const { return FLinearColor::White; }

    /**
     * Editor-only: returns a descriptive text shown in the node body.
     *
     * Subclasses should override to expose key information (speaker name,
     * event tag, condition expression, etc.).
     */
    virtual FText GetNodeDescription() const { return NodeTitle; }

    /**
     * Editor-only: Validates the node within the context of the entire graph.
     *
     * Used during asset compilation to enforce graph-wide rules
     * (e.g., Start Node must have no inputs, only one Start Node per asset).
     *
     * @param Context  Validation context providing graph-level info.
     */
    virtual void ValidateNode(class FDialogueFlowValidationContext& Context) const {}
#endif
    

    /*
     * Properties
    */

    /**
     * Unique integer ID assigned by the DialogueFlow asset.
     *
     * This ID is used to reference nodes in connections, runtime traversal,
     * serialization, and editor graph reconstruction.
     * Assigned automatically when the node is added to an asset.
     */
    UPROPERTY(VisibleAnywhere, Category = "Dialogue Node")
    int32 NodeID = -1;

    /**
     * Display name for the node (what designers see in the editor UI).
     *
     * This is also used as the default label in the graph editor unless
     * overridden by a derived node providing custom descriptive text.
     */
    UPROPERTY(EditAnywhere, Category = "Dialogue Node")
    FText NodeTitle;

    /**
     * Editor graph position used by SGraphNode or custom editor views.
     *
     * This value is not used at runtime. It only stores visual layout.
     */
    UPROPERTY()
    FVector2D GraphPosition = FVector2D::ZeroVector;

    /**
     * Array of node IDs that link *into* this node.
     *
     * Managed by the DialogueFlow asset during node connection updates.
     * Used for validation, graph traversal, and editor display.
     */
    UPROPERTY()
    TArray<int32> InputLinks;

    /**
     * Array of node IDs that this node connects *outward* to.
     *
     * Used to define graph flow and determine execution paths at runtime.
     * Typically limited (e.g., Start → 1 output, End → 0 output,
     * DialogueNode → multiple outputs, ConditionNode → two outputs).
     */
    UPROPERTY()
    TArray<int32> OutputLinks;

    /**
     * Designer-visible display name for the node type.
     *
     * Used in the editor UI to label the node category (e.g., "Start", "Dialogue").
     * Subclasses can set this in their constructors.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Dialogue Node")
    FText NodeDisplayName;

    /**
     * Color used for rendering the node in the editor.
     *
     * Subclasses can set this in their constructors to differentiate node types.
     */
    UPROPERTY(EditDefaultsOnly, Category = "Dialogue Node")
    FLinearColor NodeColor;

};
