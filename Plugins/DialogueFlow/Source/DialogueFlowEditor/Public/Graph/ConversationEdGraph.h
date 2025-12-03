// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationEdGraph.h
// Description: Custom editor graph used by the Dialogue Flow editor. Handles
//              schema assignment, start-node validation, and safe lifecycle
//              reconstruction without corrupting Slate widgets.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "ConversationEdGraph.generated.h"

class UConversationGraphStartNode;

/**
 * UConversationEdGraph
 *
 * Custom EdGraph used by the Dialogue Flow editor. Responsible for:
 * - Assigning the custom graph schema
 * - Ensuring a Start Node exists
 * - Safe PostLoad handling (NO destructive reconstruction)
 * - Syncing editor node connections to runtime node data
 *
 * NOTE:
 *   This graph deliberately avoids calling ReconstructNode() inside PostLoad()
 *   because Slate visual nodes & schema actions are not initialized yet.
 *   Calling ReconstructNode() here destroys custom SGraphNode layouts.
 */
UCLASS()
class DIALOGUEFLOWEDITOR_API UConversationEdGraph : public UEdGraph
{
    GENERATED_BODY()

public:

    UConversationEdGraph();

    /**
     * Returns the Start Node in this graph (if any).
     * Dynamically searched every time to avoid stale pointers.
     */
    UConversationGraphStartNode* FindStartNode() const;

    /**
     * Ensures a Start Node always exists.
     * Creates one if missing. Does NOT reconstruct the graph.
     */
    void EnsureStartNodeExists();

    /**
     * Called when the asset is loaded from disk.
     * Assigns the schema and ensures a Start Node exists.
     * NO pin reconstruction or graph rebuild is done here.
     */
    virtual void PostLoad() override;

    /**
     * Non-destructive validation.
     * (Used before saving or syncing.)
     */
    void ValidateGraphSafe();

    /**
     * Pushes editor graph wiring into the runtime DialogueFlow nodes.
     * Clears runtime InputLinks/OutputLinks and rebuilds from editor pins.
     */
    void SyncEditorGraphToRuntime();

    /**
     * Called when the user performs Undo/Redo.
     * Safe to call ReconstructNode() here because the editor UI is fully initialized.
     */
    virtual void PostEditUndo() override;

    /** Rebuilds the ConversationAsset->Nodes array from the EditorGraph nodes. */
    void RebuildAssetNodesFromGraph();

    /** Ensures required nodes (like Start Node) exist in the graph. */
    void EnsureRequiredNodesExist();
};
