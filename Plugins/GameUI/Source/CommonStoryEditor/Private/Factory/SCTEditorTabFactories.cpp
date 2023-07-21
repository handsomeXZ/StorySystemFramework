// Copyright Epic Games, Inc. All Rights Reserved.

#include "Factory/SCTEditorTabFactories.h"

#include "Asset/StoryChapterTree.h"
#include "Editor/StoryChapterTreeEditorToolkit.h"
#include "Containers/Array.h"
#include "Engine/Blueprint.h"
#include "GraphEditor.h"
#include "HAL/PlatformCrt.h"
#include "Internationalization/Internationalization.h"
#include "Math/Vector2D.h"
#include "Misc/AssertionMacros.h"
#include "Styling/AppStyle.h"
#include "Styling/ISlateStyle.h"
#include "Textures/SlateIcon.h"
#include "Widgets/Docking/SDockTab.h"

class SWidget;
struct FSlateBrush;

#define LOCTEXT_NAMESPACE "StoryChapterTreeEditorFactories"

//////////////////////////////////////////////////////////////////////////
// FStoryChapterTreeEditorSummoner
FStoryChapterTreeEditorSummoner::FStoryChapterTreeEditorSummoner(TSharedPtr<class FStoryChapterTreeEditor> InStoryChapterTreeEditorPtr)
	: FWorkflowTabFactory(FStoryChapterTreeEditor::StoryChapterTreeGraphEdTab, InStoryChapterTreeEditorPtr)
	, SCTEditorPtr(InStoryChapterTreeEditorPtr)
{
	TabLabel = LOCTEXT("StoryChapterTreeEditorLabel", "Editor");
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Editor");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("StoryChapterTreeEditorView", "Editor");
	ViewMenuTooltip = LOCTEXT("StoryChapterTreeEditorView_ToolTip", "Show the Editor view");
}

TSharedRef<SWidget> FStoryChapterTreeEditorSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	check(SCTEditorPtr.IsValid());
	return SCTEditorPtr.Pin()->SpawnSCTGraphEdTab();
}

FText FStoryChapterTreeEditorSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("StoryChapterTreeEditorTabTooltip", "The Story Chapter tree Editor tab allows editing of the properties of Story Chapter tree nodes");
}

//////////////////////////////////////////////////////////////////////////
// FStoryChapterTreeDetailsSummoner
FStoryChapterTreeDetailsSummoner::FStoryChapterTreeDetailsSummoner(TSharedPtr<class FStoryChapterTreeEditor> InStoryChapterTreeEditorPtr)
	: FWorkflowTabFactory(FStoryChapterTreeEditor::StoryChapterTreeDetailsTab, InStoryChapterTreeEditorPtr)
	, SCTEditorPtr(InStoryChapterTreeEditorPtr)
{
	TabLabel = LOCTEXT("StoryChapterTreeDetailsLabel", "Details");
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("StoryChapterTreeDetailsView", "Details");
	ViewMenuTooltip = LOCTEXT("StoryChapterTreeDetailsView_ToolTip", "Show the details view");
}

TSharedRef<SWidget> FStoryChapterTreeDetailsSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	check(SCTEditorPtr.IsValid());
	return SCTEditorPtr.Pin()->SpawnSCTDetailsTab();
}

FText FStoryChapterTreeDetailsSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("StoryChapterTreeDetailsTabTooltip", "The Story Chapter tree details tab allows editing of the properties of Story Chapter tree nodes");
}


//////////////////////////////////////////////////////////////////////////
// FStoryDialogueTreeEditorSummoner
FStoryDialogueTreeEditorSummoner::FStoryDialogueTreeEditorSummoner(TSharedPtr<class FStoryChapterTreeEditor> InStoryChapterTreeEditorPtr)
	: FWorkflowTabFactory(FStoryChapterTreeEditor::StoryDialogueTreeGraphEdTab, InStoryChapterTreeEditorPtr)
	, SCTEditorPtr(InStoryChapterTreeEditorPtr)
{
	TabLabel = LOCTEXT("StoryDialogueTreeDetailsLabel", "Editor");
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Editor");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("StoryDialogueTreeEditorView", "Editor");
	ViewMenuTooltip = LOCTEXT("StoryDialogueTreeEditorView_ToolTip", "Show the Editor view");
}

TSharedRef<SWidget> FStoryDialogueTreeEditorSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	check(SCTEditorPtr.IsValid());
	return SCTEditorPtr.Pin()->SpawnSDTGraphEdTab();
}

FText FStoryDialogueTreeEditorSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("StoryDialogueTreeEditorTabTooltip", "The Story Dialogue Tree Editor tab allows editing of the properties of Story Dialogue Tree nodes");
}
//////////////////////////////////////////////////////////////////////////
// FStoryChapterTreeDetailsSummoner
FStoryDialogueTreeDetailsSummoner::FStoryDialogueTreeDetailsSummoner(TSharedPtr<class FStoryChapterTreeEditor> InStoryChapterTreeEditorPtr)
	: FWorkflowTabFactory(FStoryChapterTreeEditor::StoryDialogueTreeDetailsTab, InStoryChapterTreeEditorPtr)
	, SCTEditorPtr(InStoryChapterTreeEditorPtr)
{
	TabLabel = LOCTEXT("StoryDialogueTreeDetailsLabel", "Details");
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("StoryDialogueTreeDetailsView", "Details");
	ViewMenuTooltip = LOCTEXT("StoryDialogueTreeDetailsView_ToolTip", "Show the details view");
}

TSharedRef<SWidget> FStoryDialogueTreeDetailsSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	check(SCTEditorPtr.IsValid());
	return SCTEditorPtr.Pin()->SpawnSDTDetailsTab();
}

FText FStoryDialogueTreeDetailsSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("StoryDialogueTreeDetailsTabTooltip", "The Story Dialogue tree details tab allows editing of the properties of Story Dialogue tree nodes");
}

#undef LOCTEXT_NAMESPACE
