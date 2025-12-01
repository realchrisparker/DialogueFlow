#include <Editor/ConversationEditorToolkit.h>
#include <Assets/ConversationAsset.h>
#include <Graph/ConversationEdGraph.h>

#include "PropertyEditorModule.h"
#include "Modules/ModuleManager.h"
#include "Toolkits/AssetEditorToolkit.h"

#include "GraphEditor.h"
#include "Framework/Commands/GenericCommands.h"
#include "Editor.h"

#define LOCTEXT_NAMESPACE "FConversationEditorToolkit"


// Static Tab Ids
const FName FConversationEditorToolkit::GraphTabId(TEXT("ConversationEditor_Graph"));
const FName FConversationEditorToolkit::DetailsTabId(TEXT("ConversationEditor_Details"));


// Constructor / Destructor
FConversationEditorToolkit::FConversationEditorToolkit()
    : EditingAsset(nullptr)
{
#if WITH_EDITOR
    GEditor->RegisterForUndo(this);
#endif
}

FConversationEditorToolkit::~FConversationEditorToolkit()
{
#if WITH_EDITOR
    if (GEditor)
    {
        GEditor->UnregisterForUndo(this);
    }
#endif
}


// Identification
FName FConversationEditorToolkit::GetToolkitFName() const
{
    return TEXT("ConversationEditor");
}

FText FConversationEditorToolkit::GetBaseToolkitName() const
{
    return LOCTEXT("ConversationEditor_BaseName", "Conversation Editor");
}

FText FConversationEditorToolkit::GetToolkitName() const
{
    if (EditingAsset)
    {
        return FText::FromString(FString::Printf(TEXT("Conversation: %s"), *EditingAsset->GetName()));
    }
    return LOCTEXT("ConversationEditor_DefaultName", "Conversation Editor");
}


// Initialization
void FConversationEditorToolkit::InitConversationEditor(
    const EToolkitMode::Type Mode,
    const TSharedPtr<IToolkitHost>& InitToolkitHost,
    UConversationAsset* Asset)
{
    EditingAsset = Asset;

#if WITH_EDITORONLY_DATA
    if (!EditingAsset->EditorGraph)
    {
        EditingAsset->EditorGraph = NewObject<UConversationEdGraph>(
            EditingAsset,
            UConversationEdGraph::StaticClass(),
            NAME_None,
            RF_Transactional);

        EditingAsset->EditorGraph->SetFlags(RF_Transactional);
        EditingAsset->Modify();
    }
#endif

    BuildEditorLayout();

    InitAssetEditor(
        Mode,
        InitToolkitHost,
        TEXT("ConversationEditor"),
        EditorLayout.ToSharedRef(),
        true,  // Default menu
        true,  // Default toolbar
        EditingAsset);
}


// Layout
void FConversationEditorToolkit::BuildEditorLayout()
{
    EditorLayout = FTabManager::NewLayout("ConversationEditor_Layout_v1")
        ->AddArea
        (
            FTabManager::NewPrimaryArea()
            ->SetOrientation(Orient_Horizontal)
            ->Split
            (
                FTabManager::NewStack()
                ->AddTab(GraphTabId, ETabState::OpenedTab)
                ->SetHideTabWell(true)
            )
            ->Split
            (
                FTabManager::NewStack()
                ->AddTab(DetailsTabId, ETabState::OpenedTab)
                ->SetHideTabWell(true)
            )
        );
}

// Tab Spawners
void FConversationEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

    InTabManager->RegisterTabSpawner(GraphTabId,
        FOnSpawnTab::CreateSP(this, &FConversationEditorToolkit::SpawnTab_Graph))
        .SetDisplayName(LOCTEXT("GraphTabName", "Graph"));

    InTabManager->RegisterTabSpawner(DetailsTabId,
        FOnSpawnTab::CreateSP(this, &FConversationEditorToolkit::SpawnTab_Details))
        .SetDisplayName(LOCTEXT("DetailsTabName", "Details"));

    // Make Graph the main non-closeable tab
    InTabManager->SetMainTab(FTabId(GraphTabId));
}

void FConversationEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
    InTabManager->UnregisterTabSpawner(GraphTabId);
    InTabManager->UnregisterTabSpawner(DetailsTabId);

    FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
}

// Tab Contents
TSharedRef<SDockTab> FConversationEditorToolkit::SpawnTab_Graph(const FSpawnTabArgs& Args)
{
    return SNew(SDockTab)
        .Label(LOCTEXT("GraphTabTitle", "Graph"))
        [
            CreateGraphEditorWidget()
        ];
}

TSharedRef<SDockTab> FConversationEditorToolkit::SpawnTab_Details(const FSpawnTabArgs& Args)
{
    FPropertyEditorModule& PropModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

    FDetailsViewArgs DVArgs;
    DVArgs.NotifyHook = this;
    DVArgs.bHideSelectionTip = true;

    DetailsView = PropModule.CreateDetailView(DVArgs);

    if (EditingAsset)
        DetailsView->SetObject(EditingAsset);

    return SNew(SDockTab)
        .Label(LOCTEXT("DetailsTabTitle", "Details"))
        [
            DetailsView.ToSharedRef()
        ];
}

// Graph Editor Widget
TSharedRef<SGraphEditor> FConversationEditorToolkit::CreateGraphEditorWidget()
{
    GraphEditorCommands = MakeShareable(new FUICommandList);

    // DELETE node command
    GraphEditorCommands->MapAction(
        FGenericCommands::Get().Delete,
        FExecuteAction::CreateSP(this, &FConversationEditorToolkit::HandleDeleteSelectedNodes),
        FCanExecuteAction::CreateSP(this, &FConversationEditorToolkit::CanDeleteSelectedNodes)
    );

    // Graph Events
    SGraphEditor::FGraphEditorEvents Events;
    Events.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FConversationEditorToolkit::OnGraphSelectionChanged);

    // Graph Appearance
    FGraphAppearanceInfo AppearanceInfo;
    AppearanceInfo.CornerText = FText::FromString(TEXT("Conversation"));

    // Create the Graph Editor
    SAssignNew(GraphEditor, SGraphEditor)
        .Appearance(AppearanceInfo)
        .AdditionalCommands(GraphEditorCommands)
        .IsEditable(true)
        .GraphToEdit(EditingAsset->EditorGraph)
        .GraphEvents(Events);

    return GraphEditor.ToSharedRef();
}


// Selection → Details Panel
void FConversationEditorToolkit::OnGraphSelectionChanged(
    const FGraphPanelSelectionSet& Selection)
{
    if (!DetailsView.IsValid())
        return;

    if (Selection.Num() == 1)
    {
        UObject* SelectedObj = nullptr;
        for (UObject* Obj : Selection)
            SelectedObj = Obj;

        DetailsView->SetObject(SelectedObj);
    }
    else
    {
        DetailsView->SetObject(EditingAsset);
    }
}

void FConversationEditorToolkit::RefreshDetailsPanel()
{
    if (DetailsView.IsValid())
        DetailsView->ForceRefresh();
}


// Save (Runtime Sync)
void FConversationEditorToolkit::SaveAsset_Execute()
{
#if WITH_EDITORONLY_DATA
    if (EditingAsset && EditingAsset->EditorGraph)
    {
        if (UConversationEdGraph* ConvGraph = Cast<UConversationEdGraph>(EditingAsset->EditorGraph))
        {
            ConvGraph->SyncEditorGraphToRuntime();
        }
    }
#endif

    FAssetEditorToolkit::SaveAsset_Execute();
}

// Close (Runtime Sync)
void FConversationEditorToolkit::OnClose()
{
#if WITH_EDITORONLY_DATA
    if (EditingAsset && EditingAsset->EditorGraph)
    {
        if (UConversationEdGraph* ConvGraph = Cast<UConversationEdGraph>(EditingAsset->EditorGraph))
        {
            ConvGraph->SyncEditorGraphToRuntime();
        }
    }
#endif

    FAssetEditorToolkit::OnClose();
}


// Undo / Redo (Editor-level callbacks)
void FConversationEditorToolkit::PostUndo(bool bSuccess)
{
    if (GraphEditor.IsValid())
        GraphEditor->NotifyGraphChanged();
}

void FConversationEditorToolkit::PostRedo(bool bSuccess)
{
    PostUndo(bSuccess);
}

FString FConversationEditorToolkit::GetWorldCentricTabPrefix() const
{
    return TEXT("ConversationEditor");
}

FLinearColor FConversationEditorToolkit::GetWorldCentricTabColorScale() const
{
    return FLinearColor(0.15f, 0.4f, 0.8f, 0.5f); // Any color is fine
}

void FConversationEditorToolkit::HandleDeleteSelectedNodes()
{
    if (!GraphEditor.IsValid())
        return;

    const FGraphPanelSelectionSet Selection = GraphEditor->GetSelectedNodes();

    if (Selection.Num() == 0)
        return;

    const FScopedTransaction Transaction(LOCTEXT("DeleteNodes", "Delete Conversation Nodes"));

    for (UObject* Obj : Selection)
    {
        if (UEdGraphNode* Node = Cast<UEdGraphNode>(Obj))
        {
            Node->Modify();
            Node->DestroyNode();
        }
    }
}

bool FConversationEditorToolkit::CanDeleteSelectedNodes() const
{
    return GraphEditor.IsValid()
        && GraphEditor->GetSelectedNodes().Num() > 0;
}

#undef LOCTEXT_NAMESPACE
