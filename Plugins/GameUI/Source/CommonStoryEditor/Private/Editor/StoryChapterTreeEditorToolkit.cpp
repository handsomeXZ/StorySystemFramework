#include "Editor/StoryChapterTreeEditorToolkit.h"

#include "Asset/StoryChapterTree.h"
#include "Asset/StoryDialogueTree.h"
#include "Graph/SCTGraph.h"
#include "Graph/SDTGraph.h"
#include "StoryChapterEdGraphSchema.h"
#include "StoryDialogueEdGraphSchema.h"
#include "Node/SCTGraphNode.h"
#include "Node/SCTGraphNode_Chapter.h"
#include "Node/SCTGraphNode_Transition.h"
#include "Node/SCTGraphNode_Entry.h"
#include "Node/SDTGraphNode_DEntry.h"
#include "Node/SDTGraphNode_DContent.h"
#include "Node/SDTGraphNode_DOptions.h"
#include "Node/SCTNode_Transition.h"
#include "Node/SDTNode_DContent.h"
#include "Node/SDTNode_DOptions.h"
#include "Editor/StoryEditorToolbar.h"
#include "Editor/StoryEditorCommands.h"
#include "Factory/StoryEditorModes.h"
#include "Asset/StoryAssetFactory.h"

#include "Framework/Commands/GenericCommands.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "ClassViewerModule.h"
#include "ClassViewerFilter.h"
#include "EdGraphUtilities.h"
#include "Windows/WindowsPlatformApplicationMisc.h"
#include "AssetToolsModule.h"
#include "AssetRegistry/AssetRegistryModule.h"

#define LOCTEXT_NAMESPACE "StoryChapterTreeEditor"

namespace StoryChapterTreeEditorUtils
{
	template <typename Type>
	class FNewNodeClassFilter : public IClassViewerFilter
	{
	public:
		virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
		{
			if (InClass != nullptr)
			{
				return InClass->IsChildOf(Type::StaticClass());
			}
			return false;
		}

		virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InUnloadedClassData, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
		{
			return InUnloadedClassData->IsChildOf(Type::StaticClass());
		}
	};


	/** Given a selection of nodes, return the instances that should be selected be selected for editing in the property panel */
	TArray<UObject*> GetSelectionForPropertyEditor(const TSet<UObject*>& InSelection);


}

const FName FStoryChapterTreeEditor::StoryChapterTreeMode(TEXT("StoryChapterTreeMode"));
const FName FStoryChapterTreeEditor::StoryDialogueTreeMode(TEXT("ChapterDialogueTreeMode"));
const FName FStoryChapterTreeEditor::StoryTaskTreeMode(TEXT("ChapterTaskTreeMode"));
const FName FStoryChapterTreeEditor::StoryChapterTreeGraphEdTab(TEXT("StoryChapterTreeGraphEdTab"));
const FName FStoryChapterTreeEditor::StoryDialogueTreeGraphEdTab(TEXT("StoryDialogueTreeGraphEdTab"));
const FName FStoryChapterTreeEditor::StoryChapterTreeDetailsTab(TEXT("StoryChapterTreeDetailsTab"));
const FName FStoryChapterTreeEditor::StoryDialogueTreeDetailsTab(TEXT("StoryDialogueTreeDetailsTab"));
//////////////////////////////////////////////////////////////////////////
TArray<UObject*> StoryChapterTreeEditorUtils::GetSelectionForPropertyEditor(const TSet<UObject*>& InSelection)
{
	TArray<UObject*> Selection;

	for (UObject* SelectionEntry : InSelection)
	{
		USCTGraphNode* SCTGraphNode = Cast<USCTGraphNode>(SelectionEntry);
		if (SCTGraphNode)
		{
			Selection.Add(SCTGraphNode->NodeInstance);
			continue;
		}

		USDTGraphNode* SDTGraphNode = Cast<USDTGraphNode>(SelectionEntry);
		if (SDTGraphNode)
		{
			Selection.Add(SDTGraphNode->NodeInstance);
			continue;
		}


		Selection.Add(SelectionEntry);
	}

	return Selection;

}
//////////////////////////////////////////////////////////////////////////
FStoryChapterTreeEditor::FStoryChapterTreeEditor()
	: FWorkflowCentricApplication()
	, StoryChapterTree(nullptr)
	, StoryDialogueTree(nullptr)
{
	
}

void FStoryChapterTreeEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}


void FStoryChapterTreeEditor::RegisterToolbarTab(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

void FStoryChapterTreeEditor::SetCurrentMode(FName NewMode)
{
	if (NewMode == StoryChapterTreeMode)
	{
		CurrentGraphEditor = SCTGraphEditor;
	}
	else if (NewMode == StoryDialogueTreeMode)
	{
		CurrentGraphEditor = SDTGraphEditor;
	}


	FWorkflowCentricApplication::SetCurrentMode(NewMode);
}

TSharedRef<SWidget> FStoryChapterTreeEditor::SpawnSCTDetailsTab()
{
	//加载属性编辑器模块
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::Get().LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
	//创建属性编辑器的Slate
	SCTDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	SCTDetailsView->SetIsPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled::CreateSP(this, &FStoryChapterTreeEditor::IsPropertyEditable));
	SCTDetailsView->OnFinishedChangingProperties().AddSP(this, &FStoryChapterTreeEditor::OnFinishedChangingProperties);
	//将对象传入，这样就是自动生成对象的属性面板
	SCTDetailsView->SetObject(StoryChapterTree);

	return SCTDetailsView.ToSharedRef();
}

TSharedRef<SWidget> FStoryChapterTreeEditor::SpawnSDTDetailsTab()
{
	//加载属性编辑器模块
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::Get().LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
	//创建属性编辑器的Slate
	SDTDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	SDTDetailsView->SetIsPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled::CreateSP(this, &FStoryChapterTreeEditor::IsPropertyEditable));
	SDTDetailsView->OnFinishedChangingProperties().AddSP(this, &FStoryChapterTreeEditor::OnFinishedChangingProperties);
	//将对象传入，这样就是自动生成对象的属性面板
	SDTDetailsView->SetObject(StoryDialogueTree);

	return SDTDetailsView.ToSharedRef();
}

TSharedRef<SWidget> FStoryChapterTreeEditor::SpawnSCTGraphEdTab()
{
	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FStoryChapterTreeEditor::OnSelectedNodesChanged);

	USCTGraph* EdGraph = Cast<USCTGraph>(StoryChapterTree->EdGraph);
	if (!EdGraph)
	{
		StoryChapterTree->EdGraph = FBlueprintEditorUtils::CreateNewGraph(StoryChapterTree, TEXT("Story Chapter Tree"), USCTGraph::StaticClass(), UStoryChapterEdGraphSchema::StaticClass());

		EdGraph = Cast<USCTGraph>(StoryChapterTree->EdGraph);

		EdGraph->OnCreated();
	}

	check(StoryChapterTree);
	TSharedRef<SWidget> SEditor = SAssignNew(SCTGraphEditor, SGraphEditor)
		.AdditionalCommands(ToolkitCommands)
		.GraphToEdit(StoryChapterTree->EdGraph)
		.GraphEvents(InEvents);

	CurrentGraphEditor = SCTGraphEditor;

	return SEditor;
}

TSharedRef<SWidget> FStoryChapterTreeEditor::SpawnSDTGraphEdTab()
{
	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FStoryChapterTreeEditor::OnSelectedNodesChanged);

	USDTGraph* EdGraph = Cast<USDTGraph>(StoryDialogueTree->EdGraph);
	if (!EdGraph)
	{
		StoryDialogueTree->EdGraph = FBlueprintEditorUtils::CreateNewGraph(StoryDialogueTree, TEXT("Story Dialogue Tree"), USDTGraph::StaticClass(), UStoryDialogueEdGraphSchema::StaticClass());

		EdGraph = Cast<USDTGraph>(StoryDialogueTree->EdGraph);

		EdGraph->OnCreated();
	}

	check(StoryDialogueTree);
	TSharedRef<SWidget> SEditor = SAssignNew(SDTGraphEditor, SGraphEditor)
		.AdditionalCommands(ToolkitCommands)
		// @TODO：此时除了StoryChapterTree，其他对象的EdGraph不能正确创建。
		.GraphToEdit(StoryDialogueTree->EdGraph)
		.GraphEvents(InEvents);

	CurrentGraphEditor = SDTGraphEditor;

	return SEditor;
}


void FStoryChapterTreeEditor::InitializeAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InObject)
{
	UStoryChapterTree* StoryChapterTreeToEdit = Cast<UStoryChapterTree>(InObject);
	UStoryDialogueTree* StoryDialogueTreeToEdit = Cast<UStoryDialogueTree>(InObject);

	if (StoryChapterTreeToEdit)
	{
		StoryChapterTree = StoryChapterTreeToEdit;
		StoryDialogueTree = StoryChapterTreeToEdit->EditingSDT;
	}
	else if (StoryDialogueTreeToEdit)
	{
		StoryDialogueTree = StoryDialogueTreeToEdit;
	}

	TArray<UObject*> ObjectsToEdit;
	if (StoryChapterTree != nullptr)
	{
		ObjectsToEdit.Add(StoryChapterTree);
	}
	if (StoryDialogueTree != nullptr)
	{
		ObjectsToEdit.Add(StoryDialogueTree);
	}

	if (!ToolbarBuilder.IsValid())
	{
		ToolbarBuilder = MakeShareable(new FStoryEditorToolbar(SharedThis(this)));
	}

	FStoryEditorCommonCommands::Register();

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	InitAssetEditor(Mode, InitToolkitHost, FCommonStoryEditorModule::StoryEditorAppIdentifier, FTabManager::FLayout::NullLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, ObjectsToEdit);

	BindCommonCommands();

	AddApplicationMode(StoryChapterTreeMode, MakeShareable(new FSCTEditorApplicationMode(SharedThis(this))));
	AddApplicationMode(StoryDialogueTreeMode, MakeShareable(new FSDTEditorApplicationMode(SharedThis(this))));

	if (StoryChapterTreeToEdit != nullptr)
	{
		SetCurrentMode(StoryChapterTreeMode);
	}
	else if (StoryDialogueTreeToEdit != nullptr)
	{
		SetCurrentMode(StoryDialogueTreeMode);
	}


	RegenerateMenusAndToolbars();
}


void FStoryChapterTreeEditor::BindCommonCommands()
{

	// Can't use CreateSP here because derived editor are already implementing TSharedFromThis<FAssetEditorToolkit>
	// however it should be safe, since commands are being used only within this editor
	// if it ever crashes, this function will have to go away and be reimplemented in each derived class

	ToolkitCommands->MapAction(FGenericCommands::Get().SelectAll,
		FExecuteAction::CreateRaw(this, &FStoryChapterTreeEditor::SelectAllNodes),
		FCanExecuteAction::CreateRaw(this, &FStoryChapterTreeEditor::CanSelectAllNodes)
	);

	ToolkitCommands->MapAction(FGenericCommands::Get().Delete,
		FExecuteAction::CreateRaw(this, &FStoryChapterTreeEditor::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FStoryChapterTreeEditor::CanDeleteNodes)
	);

	ToolkitCommands->MapAction(FGenericCommands::Get().Copy,
		FExecuteAction::CreateRaw(this, &FStoryChapterTreeEditor::CopySelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FStoryChapterTreeEditor::CanCopyNodes)
	);

	ToolkitCommands->MapAction(FGenericCommands::Get().Cut,
		FExecuteAction::CreateRaw(this, &FStoryChapterTreeEditor::CutSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FStoryChapterTreeEditor::CanCutNodes)
	);

	ToolkitCommands->MapAction(FGenericCommands::Get().Paste,
		FExecuteAction::CreateRaw(this, &FStoryChapterTreeEditor::PasteNodes),
		FCanExecuteAction::CreateRaw(this, &FStoryChapterTreeEditor::CanPasteNodes)
	);

	ToolkitCommands->MapAction(FGenericCommands::Get().Duplicate,
		FExecuteAction::CreateRaw(this, &FStoryChapterTreeEditor::DuplicateNodes),
		FCanExecuteAction::CreateRaw(this, &FStoryChapterTreeEditor::CanDuplicateNodes)
	);

	//ToolkitCommands->MapAction(
	//	FGraphEditorCommands::Get().CreateComment,
	//	FExecuteAction::CreateRaw(this, &FStoryChapterTreeEditor::OnCreateComment),
	//	FCanExecuteAction::CreateRaw(this, &FStoryChapterTreeEditor::CanCreateComment)
	//);

	ToolkitCommands->MapAction(FStoryEditorCommonCommands::Get().NewDialogueTree,
		FExecuteAction::CreateSP(this, &FStoryChapterTreeEditor::CreateNewDialogueTree),
		FCanExecuteAction::CreateSP(this, &FStoryChapterTreeEditor::CanCreateNewDialogueTree)
	);


}

bool FStoryChapterTreeEditor::CanAccessSDTMode() const
{
	return StoryDialogueTree != nullptr;
}

bool FStoryChapterTreeEditor::CanAccessSCTMode() const
{
	return StoryChapterTree != nullptr;
}

FText FStoryChapterTreeEditor::GetLocalizedMode(FName InMode)
{
	static TMap< FName, FText > LocModes;

	if (LocModes.Num() == 0)
	{
		LocModes.Add(StoryChapterTreeMode, LOCTEXT("StoryChapterTreeMode", "Story Chapter Tree"));
		LocModes.Add(StoryDialogueTreeMode, LOCTEXT("StoryDialogueTreeMode", "Story Dialogue Tree"));
		LocModes.Add(StoryTaskTreeMode, LOCTEXT("StoryTaskTreeMode", "Story Task Tree"));
	}

	check(InMode != NAME_None);
	const FText* OutDesc = LocModes.Find(InMode);
	check(OutDesc);
	return *OutDesc;
}

void FStoryChapterTreeEditor::CreateNewDialogueTree()
{
	FString PathName = StoryChapterTree->GetOutermost()->GetPathName();
	PathName = FPaths::GetPath(PathName);
	const FString PathNameWithFilename = PathName / LOCTEXT("NewDialogueTreeName", "NewStoryDialogueTree").ToString();

	FString Name;
	FString PackageName;
	FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
	AssetToolsModule.Get().CreateUniqueAssetName(PathNameWithFilename, TEXT(""), PackageName, Name);

	UStoryDialogueTreeFactory* Factory = NewObject<UStoryDialogueTreeFactory>();
	UStoryDialogueTree* NewAsset = Cast<UStoryDialogueTree>(AssetToolsModule.Get().CreateAssetWithDialog(Name, PathName, UStoryDialogueTree::StaticClass(), Factory));
	
	if (NewAsset != nullptr)
	{
		StoryDialogueTree = NewAsset;
		StoryChapterTree->EditingSDT = NewAsset;
	}
}

bool FStoryChapterTreeEditor::CanCreateNewDialogueTree() const
{
	return true;
}

void FStoryChapterTreeEditor::HandleNewNodeClassPicked(UClass* InClass) const
{
	if (StoryChapterTree != nullptr && InClass != nullptr && StoryChapterTree->GetOutermost())
	{
		const FString ClassName = FBlueprintEditorUtils::GetClassNameWithoutSuffix(InClass);

		FString PathName = StoryChapterTree->GetOutermost()->GetPathName();
		PathName = FPaths::GetPath(PathName);

		// Now that we've generated some reasonable default locations/names for the package, allow the user to have the final say
		// before we create the package and initialize the blueprint inside of it.
		FSaveAssetDialogConfig SaveAssetDialogConfig;
		SaveAssetDialogConfig.DialogTitleOverride = LOCTEXT("SaveAssetDialogTitle", "Save Asset As");
		SaveAssetDialogConfig.DefaultPath = PathName;
		SaveAssetDialogConfig.DefaultAssetName = ClassName + TEXT("_New");
		SaveAssetDialogConfig.ExistingAssetPolicy = ESaveAssetDialogExistingAssetPolicy::Disallow;

		const FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		const FString SaveObjectPath = ContentBrowserModule.Get().CreateModalSaveAssetDialog(SaveAssetDialogConfig);
		if (!SaveObjectPath.IsEmpty())
		{
			const FString SavePackageName = FPackageName::ObjectPathToPackageName(SaveObjectPath);
			const FString SavePackagePath = FPaths::GetPath(SavePackageName);
			const FString SaveAssetName = FPaths::GetBaseFilename(SavePackageName);

			UPackage* Package = CreatePackage(*SavePackageName);
			if (ensure(Package))
			{
				// Create and init a new Blueprint
				if (UBlueprint* NewBP = FKismetEditorUtilities::CreateBlueprint(InClass, Package, FName(*SaveAssetName), BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass()))
				{
					GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(NewBP);

					// Notify the asset registry
					FAssetRegistryModule::AssetCreated(NewBP);

					// Mark the package dirty...
					Package->MarkPackageDirty();
				}
			}
		}


	}

	FSlateApplication::Get().DismissAllMenus();
}

void FStoryChapterTreeEditor::CreateNewTransitionAction() const
{
	HandleNewNodeClassPicked(UTransitionAction::StaticClass());
}

bool FStoryChapterTreeEditor::CanCreateNewTransitionAction() const
{
	return true;
}

bool FStoryChapterTreeEditor::IsNewTransitionActionButtonVisible() const
{
	return true;
}

TSharedRef<SWidget> FStoryChapterTreeEditor::HandleCreateNewTransitionActionMenu() const
{
	FClassViewerInitializationOptions Options;
	Options.bShowUnloadedBlueprints = true;
	Options.ClassFilters.Add(MakeShareable(new StoryChapterTreeEditorUtils::FNewNodeClassFilter<UTransitionAction>));

	FOnClassPicked OnPicked(FOnClassPicked::CreateSP(this, &FStoryChapterTreeEditor::HandleNewNodeClassPicked));

	return FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassViewer(Options, OnPicked);
}

void FStoryChapterTreeEditor::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
{
	TArray<UObject*> Selection = StoryChapterTreeEditorUtils::GetSelectionForPropertyEditor(NewSelection);
	if (Selection.Num() == 1)
	{
		if (GetCurrentMode() == StoryChapterTreeMode && SCTDetailsView.IsValid())
		{
			SCTDetailsView->SetObjects(Selection);
			return;
		}
		else if (GetCurrentMode() == StoryDialogueTreeMode && SDTDetailsView.IsValid())
		{
			SDTDetailsView->SetObjects(Selection);
			return;
		}
	}
	else if (Selection.Num() == 0)
	{
		if (GetCurrentMode() == StoryChapterTreeMode && SCTDetailsView.IsValid())
		{
			SCTDetailsView->SetObject(StoryChapterTree);
			return;
		}
		else if (GetCurrentMode() == StoryDialogueTreeMode && SDTDetailsView.IsValid())
		{
			SDTDetailsView->SetObject(StoryDialogueTree);
			return;
		}
	}

	SCTDetailsView->SetObject(nullptr);
}

FGraphPanelSelectionSet FStoryChapterTreeEditor::GetSelectedNodes() const
{
	FGraphPanelSelectionSet CurrentSelection;
	if (CurrentGraphEditor)
	{
		CurrentSelection = CurrentGraphEditor->GetSelectedNodes();
	}

	return CurrentSelection;
}

void FStoryChapterTreeEditor::SelectAllNodes()
{
	if (CurrentGraphEditor)
	{
		CurrentGraphEditor->SelectAllNodes();
	}
}

bool FStoryChapterTreeEditor::CanSelectAllNodes() const
{
	return true;
}

void FStoryChapterTreeEditor::DeleteSelectedNodes()
{
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());
	CurrentGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*NodeIt))
		{
			if (Node->CanUserDeleteNode())
			{
				Node->Modify();
				Node->DestroyNode();
			}
		}
	}
}

bool FStoryChapterTreeEditor::CanDeleteNodes() const
{
	// If any of the nodes can be deleted then we should allow deleting
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanUserDeleteNode())
		{
			return true;
		}
	}

	return false;
}

void FStoryChapterTreeEditor::DeleteSelectedDuplicatableNodes()
{
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FGraphPanelSelectionSet OldSelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}

	// Delete the duplicatable nodes
	DeleteSelectedNodes();

	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}
}

void FStoryChapterTreeEditor::CutSelectedNodes()
{
	CopySelectedNodes();
	DeleteSelectedDuplicatableNodes();
}

bool FStoryChapterTreeEditor::CanCutNodes() const
{
	return CanCopyNodes() && CanDeleteNodes();
}

void FStoryChapterTreeEditor::CopySelectedNodes()
{
	// Export the selected nodes and place the text on the clipboard
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

	FString ExportedText;

	int32 CopySubNodeIndex = 0;
	for (FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node == nullptr)
		{
			SelectedIter.RemoveCurrent();
			continue;
		}

		Node->PrepareForCopying();
	}

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);

	for (FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		USCTGraphNode* Node = Cast<USCTGraphNode>(*SelectedIter);
		if (Node)
		{
			Node->PostCopyNode();
		}
	}
}

bool FStoryChapterTreeEditor::CanCopyNodes() const
{
	// If any of the nodes can be duplicated then we should allow copying
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			return true;
		}
	}

	return false;
}

void FStoryChapterTreeEditor::PasteNodes()
{
	if (CurrentGraphEditor)
	{
		PasteNodesHere(CurrentGraphEditor->GetPasteLocation());
	}
}

void FStoryChapterTreeEditor::PasteNodesHere(const FVector2D& Location)
{
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	// Undo/Redo support
	const FScopedTransaction Transaction(FGenericCommands::Get().Paste->GetDescription());
	UEdGraph* EdGraph = CurrentGraphEditor->GetCurrentGraph();
	USCTGraph* SCTGraph = Cast<USCTGraph>(EdGraph);

	EdGraph->Modify();
	if (SCTGraph)
	{
		SCTGraph->LockUpdates();
	}

	//USCTGraph* SelectedParent = NULL;
	//bool bHasMultipleNodesSelected = false;

	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	//for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	//{
	//	USCTGraphNode* Node = Cast<USCTGraphNode>(*SelectedIter);
	//	if (Node && Node->IsSubNode())
	//	{
	//		Node = Node->ParentNode;
	//	}

	//	if (Node)
	//	{
	//		if (SelectedParent == nullptr)
	//		{
	//			SelectedParent = Node;
	//		}
	//		else
	//		{
	//			bHasMultipleNodesSelected = true;
	//			break;
	//		}
	//	}
	//}

	// Clear the selection set (newly pasted stuff will be selected)
	CurrentGraphEditor->ClearSelectionSet();

	// Grab the text to paste from the clipboard.
	FString TextToImport;
	FPlatformApplicationMisc::ClipboardPaste(TextToImport);

	// Import the nodes
	TSet<UEdGraphNode*> PastedNodes;
	FEdGraphUtilities::ImportNodesFromText(EdGraph, TextToImport, /*out*/ PastedNodes);

	//Average position of nodes so we can move them while still maintaining relative distances to each other
	FVector2D AvgNodePosition(0.0f, 0.0f);

	// Number of nodes used to calculate AvgNodePosition
	int32 AvgCount = 0;

	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* EdNode = *It;
		USCTGraphNode* SCTNode = Cast<USCTGraphNode>(EdNode);
		if (EdNode)
		{
			AvgNodePosition.X += EdNode->NodePosX;
			AvgNodePosition.Y += EdNode->NodePosY;
			++AvgCount;
		}
	}

	if (AvgCount > 0)
	{
		float InvNumNodes = 1.0f / float(AvgCount);
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;
	}


	TMap<FGuid/*New*/, FGuid/*Old*/> NewToOldNodeMapping;

	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* PasteNode = *It;
		USCTGraphNode* PasteSCTNode = Cast<USCTGraphNode>(PasteNode);

		if (PasteNode)
		{
			/*bPastedParentNode = true;*/

			// Select the newly pasted stuff
			CurrentGraphEditor->SetNodeSelection(PasteNode, true);

			PasteNode->NodePosX = (PasteNode->NodePosX - AvgNodePosition.X) + Location.X;
			PasteNode->NodePosY = (PasteNode->NodePosY - AvgNodePosition.Y) + Location.Y;

			PasteNode->SnapToGrid(16);

			const FGuid OldGuid = PasteNode->NodeGuid;

			// Give new node a different Guid from the old one
			PasteNode->CreateNewGuid();

			const FGuid NewGuid = PasteNode->NodeGuid;

			NewToOldNodeMapping.Add(NewGuid, OldGuid);

			//if (PasteSCTNode)
			//{
			//	PasteSCTNode->RemoveAllSubNodes();
			//	ParentMap.Add(PasteSCTNode->CopySubNodeIndex, PasteSCTNode);
			//}
		}
	}

	//for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	//{
	//	UAIGraphNode* PasteNode = Cast<UAIGraphNode>(*It);
	//	if (PasteNode && PasteNode->IsSubNode())
	//	{
	//		PasteNode->NodePosX = 0;
	//		PasteNode->NodePosY = 0;

	//		// remove subnode from graph, it will be referenced from parent node
	//		PasteNode->DestroyNode();

	//		PasteNode->ParentNode = ParentMap.FindRef(PasteNode->CopySubNodeIndex);
	//		if (PasteNode->ParentNode)
	//		{
	//			PasteNode->ParentNode->AddSubNode(PasteNode, EdGraph);
	//		}
	//		else if (!bHasMultipleNodesSelected && !bPastedParentNode && SelectedParent)
	//		{
	//			PasteNode->ParentNode = SelectedParent;
	//			SelectedParent->AddSubNode(PasteNode, EdGraph);
	//		}
	//	}
	//}

	FixupPastedNodes(PastedNodes, NewToOldNodeMapping);

	if (SCTGraph)
	{
		//SCTGraph->UpdateClassData();
		SCTGraph->OnNodesPasted(TextToImport);
		SCTGraph->UnlockUpdates();
	}

	// Update UI
	CurrentGraphEditor->NotifyGraphChanged();

	UObject* GraphOwner = EdGraph->GetOuter();
	if (GraphOwner)
	{
		GraphOwner->PostEditChange();
		GraphOwner->MarkPackageDirty();
	}
}

void FStoryChapterTreeEditor::FixupPastedNodes(const TSet<UEdGraphNode*>& PastedGraphNodes, const TMap<FGuid/*New*/, FGuid/*Old*/>& NewToOldNodeMapping)
{
	
}

bool FStoryChapterTreeEditor::CanPasteNodes() const
{
	if (!CurrentGraphEditor.IsValid())
	{
		return false;
	}

	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	return FEdGraphUtilities::CanImportNodesFromText(CurrentGraphEditor->GetCurrentGraph(), ClipboardContent);
}

void FStoryChapterTreeEditor::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}

bool FStoryChapterTreeEditor::CanDuplicateNodes() const
{
	return CanCopyNodes();
}


FName FStoryChapterTreeEditor::GetToolkitFName() const
{
	return FName("Story Editor");
}

FText FStoryChapterTreeEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "StoryEditor");
}

FString FStoryChapterTreeEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "StoryEditor").ToString();
}

FText FStoryChapterTreeEditor::GetToolkitName() const
{
	if (GetEditingObjects().Num() > 0)
	{
		return FAssetEditorToolkit::GetLabelForObject(GetEditingObjects()[0]);
	}

	return FText();
}

FText FStoryChapterTreeEditor::GetToolkitToolTipText() const
{
	const UObject* EditingObject = (UObject*)StoryChapterTree;
	if (EditingObject != nullptr)
	{
		return FAssetEditorToolkit::GetToolTipTextForObject(EditingObject);
	}

	return FText();
}

FLinearColor FStoryChapterTreeEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.0f, 1.0f, 0.2f, 0.5f);
}

bool FStoryChapterTreeEditor::IsPropertyEditable() const
{
	return true;
}

void FStoryChapterTreeEditor::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	TSharedPtr<SGraphEditor> MyGraphEditor;
	if (GetCurrentMode() == StoryChapterTreeMode && SCTGraphEditor.IsValid())
	{
		MyGraphEditor = SCTGraphEditor;
	}
	else if (GetCurrentMode() == StoryDialogueTreeMode && SDTGraphEditor.IsValid())
	{
		MyGraphEditor = SDTGraphEditor;
	}
	else
	{
		return;
	}
	
	
	UEdGraph* EdGraph = MyGraphEditor->GetCurrentGraph();
	USCTGraph* SCTGraph = Cast<USCTGraph>(EdGraph);
	if (SCTGraph)
	{
		//SCTGraph->UpdateAsset();
	}
	
	// Hope to Rebuild Widget(A mistake?)
	if (MyGraphEditor.IsValid() && MyGraphEditor->GetCurrentGraph())
	{
		MyGraphEditor->GetCurrentGraph()->GetSchema()->ForceVisualizationCacheClear();
	}
}

void FStoryChapterTreeEditor::SaveAsset_Execute()
{
	if (StoryChapterTree)
	{
		USCTGraph* EdGraph = Cast<USCTGraph>(StoryChapterTree->EdGraph);
		if (EdGraph)
		{
			EdGraph->OnSave();
		}
	}

	if (StoryDialogueTree)
	{
		USDTGraph* EdGraph = Cast<USDTGraph>(StoryDialogueTree->EdGraph);
		if (EdGraph)
		{
			EdGraph->OnSave();
		}
	}
	// save it
	FAssetEditorToolkit::SaveAsset_Execute();
}


#undef LOCTEXT_NAMESPACE