// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationEditorToolkit.h
// Description: Main editor toolkit for ConversationAsset. Handles tab layout,
//              graph editor, details panel, runtime sync, and undo/redo hooks.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Misc/NotifyHook.h"
#include "EditorUndoClient.h" 

class UConversationAsset;
class UConversationEdGraph;
class SGraphEditor;
class IDetailsView;


/**
 * Editor toolkit for Conversation Assets.
 * Provides:
 *  - Graph editing interface
 *  - Details panel
 *  - Undo/Redo handling via FEditorUndoClient
 *  - Runtime sync on save/close
 *  - Stable two-tab layout
 */
class FConversationEditorToolkit
    : public FAssetEditorToolkit
    , public FNotifyHook
    , public FEditorUndoClient 
{
public:

    FConversationEditorToolkit();
    virtual ~FConversationEditorToolkit();

    /** Initializes the editor for a Conversation Asset */
    void InitConversationEditor(
        const EToolkitMode::Type Mode,
        const TSharedPtr<IToolkitHost>& InitToolkitHost,
        UConversationAsset* Asset);

    // -------- FAssetEditorToolkit overrides --------
    virtual FName GetToolkitFName() const override;
    virtual FText GetBaseToolkitName() const override;
    virtual FText GetToolkitName() const override;
    virtual void SaveAsset_Execute() override;
    virtual void OnClose() override;

    // -------- Tab Management --------
    virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
    virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;

    TSharedRef<SDockTab> SpawnTab_Graph(const FSpawnTabArgs& Args);
    TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);

    virtual FString GetWorldCentricTabPrefix() const override;
    virtual FLinearColor GetWorldCentricTabColorScale() const override;

    // -------- Undo/Redo Handling --------
    virtual void PostUndo(bool bSuccess) override;     // << OVERRIDE NOW VALID
    virtual void PostRedo(bool bSuccess) override;     // << OVERRIDE NOW VALID

    // -------- Graph/Details --------
    void OnGraphSelectionChanged(const FGraphPanelSelectionSet& Selection);
    void RefreshDetailsPanel();

private:

    TSharedRef<SGraphEditor> CreateGraphEditorWidget();
    void BuildEditorLayout();

private:

    TObjectPtr<UConversationAsset> EditingAsset;

    TSharedPtr<SGraphEditor> GraphEditor;
    TSharedPtr<IDetailsView> DetailsView;

    TSharedPtr<FTabManager::FLayout> EditorLayout;
    TSharedPtr<FUICommandList> GraphEditorCommands;

    static const FName GraphTabId;
    static const FName DetailsTabId;

    /** DELETE command handler */
    void HandleDeleteSelectedNodes();
    bool CanDeleteSelectedNodes() const;
};
