// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationEditorToolkit.h
// Description: Editor toolkit for editing Conversation Assets. 
//              Provides the graph editor tab and future extensible UI tabs.
// ============================================================================

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"

class UConversationAsset;
class UEdGraph;
class SGraphEditor;

/**
 * FConversationEditorToolkit
 *
 * Standalone editor used by the engine to open and edit
 * UConversationAsset instances. This toolkit manages window layout,
 * tab registration, menus/toolbars, and creation of the graph editor.
 */
class FConversationEditorToolkit : public FAssetEditorToolkit, public FNotifyHook
{
public:

    /** Constructor */
    FConversationEditorToolkit();

    /** Destructor */
    virtual ~FConversationEditorToolkit() override;

    /**
     * Initialize the editor for a Conversation Asset.
     *
     * @param InitToolkitHost   Optional host if being used world-centrically.
     * @param Asset             The Conversation Asset this editor will modify.
     */
    void InitConversationEditor(const TSharedPtr<class IToolkitHost>& InitToolkitHost, UConversationAsset* Asset);

    /*
     *FAssetEditorToolkit Interface
     */

     /** Returns the internal name of this toolkit */
    virtual FName GetToolkitFName() const override;

    /** Returns the label that appears in the editor tab */
    virtual FText GetBaseToolkitName() const override;

    /** Returns the full name including asset being edited */
    virtual FText GetToolkitName() const override;

    /** Prefix used when this editor is world-centric */
    virtual FString GetWorldCentricTabPrefix() const override;

    /** Color used when this editor is displayed world-centrically */
    virtual FLinearColor GetWorldCentricTabColorScale() const override;

    virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;

    virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;

    virtual void SaveAsset_Execute() override;
    
    void OnGraphSelectionChanged(const FGraphPanelSelectionSet& Selection);

    /*
     *  Tabs
    */

    /** Tab ID for the Graph Editor tab */
    static const FName GraphEditorTabId;

    /** Tab ID for the Details tab */
    static const FName DetailsTabID;

protected:

    /**
     * Creates the Slate graph editor widget.
     * This binds the UEdGraph stored inside the asset to SGraphEditor.
     */
    TSharedRef<SWidget> CreateGraphEditor();

    /**
     * Called by the TabManager when the Graph tab is created.
     *
     * @param Args Spawn arguments describing how the tab should be created.
     */
    TSharedRef<SDockTab> SpawnGraphEditorTab(const FSpawnTabArgs& Args);

    /**
     * Called by the TabManager when the Details tab is created.
     *
     * @param Args Spawn arguments describing how the tab should be created.
     */
    TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);

private:

    /** The ConversationAsset currently being edited */
    UConversationAsset* EditingAsset = nullptr;

    /** The UEdGraph used by the graph editor */
    UEdGraph* EditingGraph = nullptr;

    TSharedPtr<IDetailsView> DetailsView;

    /** Slate graph editor widget instance */
    TSharedPtr<SGraphEditor> GraphEditor;

    TSharedPtr<FWorkspaceItem> WorkspaceMenuCategory;
};
