// ============================================================================
// Copyright © 2025 God's Studio
// All Rights Reserved.
//
// Project: Dialogue Flow
// File: ConversationEditorToolkit.cpp
// Description: Implementation of the Conversation Editor toolkit. 
//              Creates the graph editor and manages layout, tabs, and the
//              editing of UConversationAsset instances.
// ============================================================================

#include <Editor/ConversationEditorToolkit.h>
#include <Assets/ConversationAsset.h>
#include <Graph/ConversationEdGraph.h>
#include <Graph/ConversationGraphSchema.h>

#include "Editor.h"
#include "Editor/UnrealEdEngine.h"
#include "UnrealEdGlobals.h"
#include "FileHelpers.h"
#include "Framework/Commands/GenericCommands.h"


#define LOCTEXT_NAMESPACE "FConversationEditorToolkit"

// Static Tab ID
const FName FConversationEditorToolkit::GraphEditorTabId(TEXT("ConversationEditor_Graph"));

// Static Tab ID for Details
const FName FConversationEditorToolkit::DetailsTabID(TEXT("ConversationEditor_Details"));


// Constructor
FConversationEditorToolkit::FConversationEditorToolkit()
{
}

// Destructor
FConversationEditorToolkit::~FConversationEditorToolkit()
{
}


// Initialize Editor
void FConversationEditorToolkit::InitConversationEditor(const TSharedPtr<IToolkitHost>& InitToolkitHost, UConversationAsset* Asset)
{
    check(Asset);

    EditingAsset = Asset;

    // Ensure the asset has a valid graph
    if (EditingAsset->EditorGraph == nullptr)
    {
        EditingAsset->EditorGraph = NewObject<UConversationEdGraph>(
            EditingAsset,
            UConversationEdGraph::StaticClass(),
            NAME_None,
            RF_Transactional
        );

        // Assign Schema
        EditingAsset->EditorGraph->Schema = UConversationGraphSchema::StaticClass();
        EditingAsset->Modify();
    }

    EditingGraph = EditingAsset->EditorGraph;
    check(EditingGraph);

    if (EditingGraph->Schema != UConversationGraphSchema::StaticClass())
    {
        EditingGraph->Schema = UConversationGraphSchema::StaticClass();
        EditingGraph->Modify();
    }

    // Restore or create Start Node
    CastChecked<UConversationEdGraph>(EditingGraph)->RebuildGraph();

    // Validate graph structure
    CastChecked<UConversationEdGraph>(EditingGraph)->ValidateGraph();

    //  Tab Layout Setup
    const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("ConversationEditor_Layout_v1")
        ->AddArea
        (
            FTabManager::NewPrimaryArea()
            ->SetOrientation(Orient_Horizontal)
            ->Split
            (
                FTabManager::NewStack()
                ->SetSizeCoefficient(0.7f)
                ->AddTab(GraphEditorTabId, ETabState::OpenedTab)
                ->SetHideTabWell(true)
            )
            ->Split
            (
                FTabManager::NewStack()
                ->SetSizeCoefficient(0.3f)
                ->AddTab(DetailsTabID, ETabState::OpenedTab)
            )
        );

    WorkspaceMenuCategory = FWorkspaceItem::NewGroup(LOCTEXT("WorkspaceMenu_ConversationEditor", "Conversation Editor"));

    // Register Tab Spawners
    FAssetEditorToolkit::InitAssetEditor(
        EToolkitMode::Standalone,
        InitToolkitHost,
        FName("ConversationEditor"),
        Layout,
        true,
        true,
        EditingAsset
    );

    RegenerateMenusAndToolbars();
}

//  Editor UI: Spawn Graph Editor Tab
TSharedRef<SDockTab> FConversationEditorToolkit::SpawnGraphEditorTab(const FSpawnTabArgs& Args)
{
    check(Args.GetTabId() == GraphEditorTabId);

    return SNew(SDockTab)
        .Label(LOCTEXT("GraphEditorTab", "Conversation Graph"))
        [
            CreateGraphEditor()
        ];
}

// Editor UI: Spawn Details Tab
TSharedRef<SDockTab> FConversationEditorToolkit::SpawnTab_Details(const FSpawnTabArgs& Args)
{
    check(Args.GetTabId() == DetailsTabID);

    FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

    FDetailsViewArgs DetailsArgs;
    DetailsArgs.bHideSelectionTip = true;
    DetailsArgs.NotifyHook = this;

    DetailsView = PropertyEditorModule.CreateDetailView(DetailsArgs);

    // If nothing selected, show the asset
    if (EditingAsset)
    {
        DetailsView->SetObject(EditingAsset);
    }

    return SNew(SDockTab)
        .Label(FText::FromString("Details"))
        [
            DetailsView.ToSharedRef()
        ];
}

//  Create SGraphEditor
TSharedRef<SWidget> FConversationEditorToolkit::CreateGraphEditor()
{
    check(EditingGraph);

    FGraphAppearanceInfo AppearanceInfo;
    AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText", "Conversation Graph");

    // Setup events

    SGraphEditor::FGraphEditorEvents Events;

    Events.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FConversationEditorToolkit::OnGraphSelectionChanged);

    Events.OnNodeDoubleClicked =
        FSingleNodeEvent::CreateLambda(
            [ ] (UEdGraphNode*) {}
        );

    // Create the graph editor
    GraphEditor = SNew(SGraphEditor)
        .GraphToEdit(EditingGraph)
        .Appearance(AppearanceInfo)
        .GraphEvents(Events)
        .IsEditable(true);

    return GraphEditor.ToSharedRef();
}

void FConversationEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

    // Register Graph Editor Tab
    InTabManager->RegisterTabSpawner(GraphEditorTabId,
        FOnSpawnTab::CreateRaw(this, &FConversationEditorToolkit::SpawnGraphEditorTab))
        .SetDisplayName(LOCTEXT("GraphEditorTabLabel", "Conversation Graph"))
        .SetGroup(WorkspaceMenuCategory.ToSharedRef());
    InTabManager->SetMainTab(FTabId(GraphEditorTabId)); // Ensure this tab can not be closed.

    // Register Details Tab
    InTabManager->RegisterTabSpawner(DetailsTabID,
        FOnSpawnTab::CreateSP(this, &FConversationEditorToolkit::SpawnTab_Details))
        .SetDisplayName(FText::FromString("Details"))
        .SetGroup(WorkspaceMenuCategory.ToSharedRef())
        .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FConversationEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
    InTabManager->UnregisterTabSpawner(GraphEditorTabId);
    InTabManager->UnregisterTabSpawner(DetailsTabID);
}

void FConversationEditorToolkit::OnGraphSelectionChanged(const FGraphPanelSelectionSet& Selection)
{
    if (Selection.Num() == 1)
    {
        UObject* SelectedObj = nullptr;
        for (UObject* Obj : Selection)
        {
            SelectedObj = Obj;
        }

        // If a graph node was selected, display its runtime UObject instead.
        if (UConversationGraphNode* GraphNode = Cast<UConversationGraphNode>(SelectedObj))
        {
            if (UObject* PropertyObj = GraphNode->GetPropertyObject())
            {
                DetailsView->SetObject(PropertyObj);
                return;
            }
        }

        // Otherwise, show the raw selected object.
        DetailsView->SetObject(SelectedObj);
    }
    else
    {
        DetailsView->SetObject(EditingAsset);
    }
}

void FConversationEditorToolkit::SaveAsset_Execute()
{
    if (!EditingAsset)
    {
        return;
    }

    EditingAsset->MarkPackageDirty();

    TArray<UPackage*> Packages;
    Packages.Add(EditingAsset->GetOutermost());

    UEditorLoadingAndSavingUtils::SavePackages(Packages, false);
}

/*
 * Toolkit Metadata
*/

FName FConversationEditorToolkit::GetToolkitFName() const
{
    return FName("ConversationEditor");
}

FText FConversationEditorToolkit::GetBaseToolkitName() const
{
    return LOCTEXT("ConversationEditorLabel", "Conversation Editor");
}

FText FConversationEditorToolkit::GetToolkitName() const
{
    return FText::Format(
        LOCTEXT("ConversationEditorToolkitName", "{0} - Conversation"),
        FText::FromString(EditingAsset->GetName())
    );
}

FString FConversationEditorToolkit::GetWorldCentricTabPrefix() const
{
    return TEXT("Conversation");
}

FLinearColor FConversationEditorToolkit::GetWorldCentricTabColorScale() const
{
    return FLinearColor(0.7f, 0.2f, 1.0f);
}

#undef LOCTEXT_NAMESPACE