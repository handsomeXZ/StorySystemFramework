// Copyright Epic Games, Inc. All Rights Reserved.

#include "Factory/StoryEditorModes.h"

#include "Factory/SCTEditorTabFactories.h"
#include "Editor/StoryEditorToolbar.h"

#include "Editor/StoryChapterTreeEditorToolkit.h"
#include "Framework/Docking/TabManager.h"
#include "Misc/AssertionMacros.h"
#include "Types/SlateEnums.h"

/////////////////////////////////////////////////////
// FSCTEditorApplicationMode

#define LOCTEXT_NAMESPACE "StoryChapterTreeApplicationMode"

FSCTEditorApplicationMode::FSCTEditorApplicationMode(TSharedPtr<class FStoryChapterTreeEditor> InStoryChapterTreeEditor)
	: FApplicationMode(FStoryChapterTreeEditor::StoryChapterTreeMode, FStoryChapterTreeEditor::GetLocalizedMode)
{
	SCTEditor = InStoryChapterTreeEditor;

	SCTTabFactories.RegisterFactory(MakeShareable(new FStoryChapterTreeEditorSummoner(InStoryChapterTreeEditor)));
	SCTTabFactories.RegisterFactory(MakeShareable(new FStoryChapterTreeDetailsSummoner(InStoryChapterTreeEditor)));

	// The TabLayout will be overwritten by the externally saved INI. 
	// No solution has been found. The TabLayout can only be updated by modifying the LayoutName
	TabLayout = FTabManager::NewLayout( "Standalone_StoryChapterTree_Layout_V3" )
	->AddArea
	(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.7f)
				->AddTab(FStoryChapterTreeEditor::StoryChapterTreeGraphEdTab, ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.3f)
				->AddTab(FStoryChapterTreeEditor::StoryChapterTreeDetailsTab, ETabState::OpenedTab)
			)
		)
	);
	
	InStoryChapterTreeEditor->GetToolbarBuilder()->AddModesToolbar(ToolbarExtender);
	InStoryChapterTreeEditor->GetToolbarBuilder()->AddStoryChapterTreeToolbar(ToolbarExtender);
}

void FSCTEditorApplicationMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	check(SCTEditor.IsValid());
	TSharedPtr<FStoryChapterTreeEditor> SCTEditorPtr = SCTEditor.Pin();
	
	SCTEditorPtr->RegisterToolbarTab(InTabManager.ToSharedRef());

	// Mode-specific setup
	SCTEditorPtr->PushTabFactories(SCTTabFactories);

	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FSCTEditorApplicationMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();

	check(SCTEditor.IsValid());
	TSharedPtr<FStoryChapterTreeEditor> SCTEditorPtr = SCTEditor.Pin();
	
	/*SCTEditorPtr->SaveEditedObjectState();*/
}

void FSCTEditorApplicationMode::PostActivateMode()
{

	FApplicationMode::PostActivateMode();
}

#undef  LOCTEXT_NAMESPACE

/////////////////////////////////////////////////////
// FSDTEditorApplicationMode

#define LOCTEXT_NAMESPACE "StoryDialogueTreeApplicationMode"

FSDTEditorApplicationMode::FSDTEditorApplicationMode(TSharedPtr<class FStoryChapterTreeEditor> InStoryChapterTreeEditor)
	: FApplicationMode(FStoryChapterTreeEditor::StoryDialogueTreeMode, FStoryChapterTreeEditor::GetLocalizedMode)
{
	SCTEditor = InStoryChapterTreeEditor;
	
	SDTTabFactories.RegisterFactory(MakeShareable(new FStoryDialogueTreeEditorSummoner(InStoryChapterTreeEditor)));
	SDTTabFactories.RegisterFactory(MakeShareable(new FStoryDialogueTreeDetailsSummoner(InStoryChapterTreeEditor)));

	// The DetailsView can be shared
	/*SDTTabFactories.RegisterFactory(MakeShareable(new FStoryDialogueDetailsSummoner(InStoryChapterTreeEditor)));*/

	TabLayout = FTabManager::NewLayout("Standalone_StoryDialogueTree_Layout_V3")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.7f)
					->AddTab(FStoryChapterTreeEditor::StoryDialogueTreeGraphEdTab, ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->SetSizeCoefficient(0.3f)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.6f)
						->AddTab(FStoryChapterTreeEditor::StoryDialogueTreeDetailsTab, ETabState::OpenedTab)
					)
				)
			)
		);

	InStoryChapterTreeEditor->GetToolbarBuilder()->AddModesToolbar(ToolbarExtender);
	InStoryChapterTreeEditor->GetToolbarBuilder()->AddStoryDialogueTreeToolbar(ToolbarExtender);
}

void FSDTEditorApplicationMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	check(SCTEditor.IsValid());
	TSharedPtr<FStoryChapterTreeEditor> StoryChapterTreeEditorPtr = SCTEditor.Pin();
	
	StoryChapterTreeEditorPtr->RegisterToolbarTab(InTabManager.ToSharedRef());

	// Mode-specific setup
	StoryChapterTreeEditorPtr->PushTabFactories(SDTTabFactories);

	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FSDTEditorApplicationMode::PostActivateMode()
{
	// Reopen any documents that were open when the BT was last saved
	check(SCTEditor.IsValid());
	TSharedPtr<FStoryChapterTreeEditor> StoryChapterTreeEditorPtr = SCTEditor.Pin();
	//	StoryChapterTreeEditorPtr->StartEditingDefaults();

	FApplicationMode::PostActivateMode();
}

#undef LOCTEXT_NAMESPACE
