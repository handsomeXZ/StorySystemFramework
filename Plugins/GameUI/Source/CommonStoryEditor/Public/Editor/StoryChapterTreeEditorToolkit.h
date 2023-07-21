#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"


class COMMONSTORYEDITOR_API FStoryChapterTreeEditor : public FWorkflowCentricApplication
{
public:
	FStoryChapterTreeEditor();
	virtual ~FStoryChapterTreeEditor() {};

	static const FName StoryChapterTreeGraphEdTab;
	static const FName StoryDialogueTreeGraphEdTab;
	static const FName StoryChapterTreeDetailsTab;
	static const FName StoryDialogueTreeDetailsTab;
	/** Modes in mode switcher */
	static const FName StoryChapterTreeMode;
	static const FName StoryDialogueTreeMode;
	static const FName StoryTaskTreeMode;

	//~ Begin IToolkit Interface
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FText GetToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	//~ End IToolkit Interface

	//~ Begin FWorkflowCentricApplication Interface
	virtual void SetCurrentMode(FName NewMode) override;
	//~ End FWorkflowCentricApplication Interface

	// @todo This is a hack for now until we reconcile the default toolbar with application modes [duplicated from counterpart in Blueprint Editor]
	void RegisterToolbarTab(const TSharedRef<class FTabManager>& TabManager);

	void InitializeAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InObject);
	/** Restores the behavior tree graph we were editing or creates a new one if none is available */
	void RestoreStoryChapterTree();

	/** Called when the selection changes in the GraphEditor */
	void OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection);
	FGraphPanelSelectionSet GetSelectedNodes() const;
	virtual void BindCommonCommands();
	// Delegates for graph editor commands
	void SelectAllNodes();
	bool CanSelectAllNodes() const;
	void DeleteSelectedNodes();
	bool CanDeleteNodes() const;
	void DeleteSelectedDuplicatableNodes();
	void CutSelectedNodes();
	bool CanCutNodes() const;
	void CopySelectedNodes();
	bool CanCopyNodes() const;
	void PasteNodes();
	void PasteNodesHere(const FVector2D& Location);
	bool CanPasteNodes() const;
	void DuplicateNodes();
	bool CanDuplicateNodes() const;
	
	TSharedRef<SWidget> SpawnSCTDetailsTab();
	TSharedRef<SWidget> SpawnSCTGraphEdTab();
	TSharedRef<SWidget> SpawnSDTDetailsTab();
	TSharedRef<SWidget> SpawnSDTGraphEdTab();

	bool IsPropertyEditable() const;
	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);

	/** 
	 * Get the localized text to display for the specified mode 
	 * @param	InMode	The mode to display
	 * @return the localized text representation of the mode
	 */
	static FText GetLocalizedMode(FName InMode);

	/** Access the toolbar builder for this editor */
	TSharedPtr<class FStoryEditorToolbar> GetToolbarBuilder() { return ToolbarBuilder; }

	void HandleNewNodeClassPicked(UClass* InClass) const;

	void CreateNewDialogueTree();
	void CreateNewTransitionAction() const;
	bool CanCreateNewDialogueTree() const;
	bool CanCreateNewTransitionAction() const;
	bool IsNewTransitionActionButtonVisible() const;
	TSharedRef<SWidget> HandleCreateNewTransitionActionMenu() const;

	bool CanAccessSCTMode() const;
	bool CanAccessSDTMode() const;

protected:
	/** Called when "Save" is clicked for this asset */
	virtual void SaveAsset_Execute() override;

	void FixupPastedNodes(const TSet<UEdGraphNode*>& NewPastedGraphNodes, const TMap<FGuid/*New*/, FGuid/*Old*/>& NewToOldNodeMapping);
	
private:
	/** Tab's Content: SGraphEditor */
	TSharedPtr<SGraphEditor> SCTGraphEditor;
	TSharedPtr<SGraphEditor> SDTGraphEditor;
	TSharedPtr<SGraphEditor> CurrentGraphEditor;

	/** Property Details View */
	TSharedPtr<class IDetailsView> SCTDetailsView;
	TSharedPtr<class IDetailsView> SDTDetailsView;

	/* The Asset being edited */
	class UStoryChapterTree* StoryChapterTree;
	class UStoryDialogueTree* StoryDialogueTree;


	/** The command list for this editor */
	TSharedPtr<FUICommandList> GraphEditorCommands;

	TSharedPtr<class FStoryEditorToolbar> ToolbarBuilder;

};