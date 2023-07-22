// Copyright Epic Games, Inc. All Rights Reserved.

#include "Editor/StoryEditorToolbar.h"

#include "Editor/StoryChapterTreeEditorToolkit.h"
#include "Editor/StoryEditorCommands.h"
#include "Editor/StoryEditorToolbar.h"

#include "Delegates/Delegate.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "Framework/SlateDelegates.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Text.h"
#include "Layout/Margin.h"
#include "Math/Vector2D.h"
#include "Misc/AssertionMacros.h"
#include "Misc/Attribute.h"
#include "Styling/AppStyle.h"
#include "Textures/SlateIcon.h"
#include "UObject/NameTypes.h"
#include "UObject/UnrealNames.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Text/STextBlock.h"
#include "WorkflowOrientedApp/SModeWidget.h"

class SWidget;

#define LOCTEXT_NAMESPACE "StoryEditorToolbar"

class SStoryTreeModeSeparator : public SBorder
{
public:
	SLATE_BEGIN_ARGS(SStoryTreeModeSeparator) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArg)
	{
		SBorder::Construct(
			SBorder::FArguments()
			.BorderImage(FAppStyle::GetBrush("BlueprintEditor.PipelineSeparator"))
			.Padding(0.0f)
			);
	}

	// SWidget interface
	virtual FVector2D ComputeDesiredSize(float) const override
	{
		const float Height = 20.0f;
		const float Thickness = 16.0f;
		return FVector2D(Thickness, Height);
	}
	// End of SWidget interface
};

void FStoryEditorToolbar::AddModesToolbar(TSharedPtr<FExtender> Extender)
{
	check(StoryEditor.IsValid());
	TSharedPtr<FStoryChapterTreeEditor> StoryEditorPtr = StoryEditor.Pin();

	Extender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		StoryEditorPtr->GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateSP( this, &FStoryEditorToolbar::FillModesToolbar ) );
}


void FStoryEditorToolbar::AddStoryChapterTreeToolbar(TSharedPtr<FExtender> Extender)
{
	check(StoryEditor.IsValid());
	TSharedPtr<FStoryChapterTreeEditor> StoryEditorPtr = StoryEditor.Pin();

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, StoryEditorPtr->GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP( this, &FStoryEditorToolbar::FillStoryChapterTreeToolbar ));
	StoryEditorPtr->AddToolbarExtender(ToolbarExtender);
}

void FStoryEditorToolbar::AddStoryDialogueTreeToolbar(TSharedPtr<FExtender> Extender)
{
	check(StoryEditor.IsValid());
	TSharedPtr<FStoryChapterTreeEditor> StoryEditorPtr = StoryEditor.Pin();

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, StoryEditorPtr->GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP(this, &FStoryEditorToolbar::FillStoryDialogueTreeToolbar));
	StoryEditorPtr->AddToolbarExtender(ToolbarExtender);
}

void FStoryEditorToolbar::FillModesToolbar(FToolBarBuilder& ToolbarBuilder)
{
	check(StoryEditor.IsValid());
	TSharedPtr<FStoryChapterTreeEditor> StoryEditorPtr = StoryEditor.Pin();

	TAttribute<FName> GetActiveMode(StoryEditorPtr.ToSharedRef(), &FStoryChapterTreeEditor::GetCurrentMode);
	FOnModeChangeRequested SetActiveMode = FOnModeChangeRequested::CreateSP(StoryEditorPtr.ToSharedRef(), &FStoryChapterTreeEditor::SetCurrentMode);

	// Left side padding
	StoryEditorPtr->AddToolbarWidget(SNew(SSpacer).Size(FVector2D(4.0f, 1.0f)));
	
	StoryEditorPtr->AddToolbarWidget(
		SNew(SModeWidget, FStoryChapterTreeEditor::GetLocalizedMode( FStoryChapterTreeEditor::StoryChapterTreeMode), FStoryChapterTreeEditor::StoryChapterTreeMode)
		.OnGetActiveMode(GetActiveMode)
		.OnSetActiveMode(SetActiveMode)
		.CanBeSelected(StoryEditorPtr.Get(), &FStoryChapterTreeEditor::CanAccessSCTMode)
		.ToolTipText(LOCTEXT("StoryChapterTreeModeButtonTooltip", "Switch to Story Chapter Tree Mode"))
		.IconImage(FAppStyle::GetBrush("BTEditor.SwitchToBehaviorTreeMode"))
	);

	StoryEditorPtr->AddToolbarWidget(SNew(SSpacer).Size(FVector2D(10.0f, 1.0f)));

	StoryEditorPtr->AddToolbarWidget(
		SNew(SModeWidget, FStoryChapterTreeEditor::GetLocalizedMode( FStoryChapterTreeEditor::StoryDialogueTreeMode ), FStoryChapterTreeEditor::StoryDialogueTreeMode)
		.OnGetActiveMode(GetActiveMode)
		.OnSetActiveMode(SetActiveMode)
		.CanBeSelected(StoryEditorPtr.Get(), &FStoryChapterTreeEditor::CanAccessSDTMode)
		.ToolTipText(LOCTEXT("StoryDialogueTreeModeButtonTooltip", "Switch to Story Dialogue Tree Mode"))
		.IconImage(FAppStyle::GetBrush("BTEditor.SwitchToBehaviorTreeMode"))
	);
		
	// Right side padding
	StoryEditorPtr->AddToolbarWidget(SNew(SSpacer).Size(FVector2D(10.0f, 1.0f)));
}

void FStoryEditorToolbar::FillStoryChapterTreeToolbar(FToolBarBuilder& ToolbarBuilder)
{
	check(StoryEditor.IsValid());
	TSharedPtr<FStoryChapterTreeEditor> StoryEditorPtr = StoryEditor.Pin();
	if (StoryEditorPtr->GetCurrentMode() == FStoryChapterTreeEditor::StoryChapterTreeMode)
	{
		ToolbarBuilder.BeginSection("Dialogue");
		{
			ToolbarBuilder.AddToolBarButton(FStoryEditorCommonCommands::Get().NewDialogueTree, NAME_None, TAttribute<FText>(), TAttribute<FText>(),
				TAttribute<FSlateIcon>(FSlateIcon(FAppStyle::Get().GetStyleSetName(), "BTEditor.SwitchToBehaviorTreeMode")), FName(TEXT("NewDialogueTree")));
		}
		ToolbarBuilder.EndSection();

		ToolbarBuilder.BeginSection("TransitionAction");
		{
			const FText NewTransitionActionLabel = LOCTEXT("NewTransitionAction_Label", "New Transition Action");
			const FText NewTransitionActionTooltip = LOCTEXT("NewTransitionAction_ToolTip", "Create a new Transition Action Blueprint from a base class");
			const FSlateIcon NewTransitionActionIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "BTEditor.Graph.NewTask");

			ToolbarBuilder.AddToolBarButton(
				FUIAction(
					FExecuteAction::CreateSP(StoryEditorPtr.Get(), &FStoryChapterTreeEditor::CreateNewTransitionAction),
					FCanExecuteAction::CreateSP(StoryEditorPtr.Get(), &FStoryChapterTreeEditor::CanCreateNewTransitionAction),
					FIsActionChecked(),
					FIsActionButtonVisible::CreateSP(StoryEditorPtr.Get(), &FStoryChapterTreeEditor::IsNewTransitionActionButtonVisible)
				),
				NAME_None,
				NewTransitionActionLabel,
				NewTransitionActionTooltip,
				NewTransitionActionIcon
			);

			//ToolbarBuilder.AddComboButton(
			//	FUIAction(
			//		FExecuteAction(),
			//		FCanExecuteAction::CreateSP(StoryEditorPtr.Get(), &FStoryChapterTreeEditor::CanCreateNewTransitionAction),
			//		FIsActionChecked(),
			//		FIsActionButtonVisible::CreateSP(StoryEditorPtr.Get(), &FStoryChapterTreeEditor::IsNewTransitionActionComboVisible)
			//	),
			//	FOnGetContent::CreateSP(StoryEditorPtr.Get(), &FStoryChapterTreeEditor::HandleCreateNewTransitionActionMenu),
			//	NewTransitionActionLabel,
			//	NewTransitionActionTooltip,
			//	NewTransitionActionIcon
			//);
		}
		ToolbarBuilder.EndSection();
	}
}

void FStoryEditorToolbar::FillStoryDialogueTreeToolbar(FToolBarBuilder& ToolbarBuilder)
{
	check(StoryEditor.IsValid());
	TSharedPtr<FStoryChapterTreeEditor> StoryEditorPtr = StoryEditor.Pin();
	if (StoryEditorPtr->GetCurrentMode() == FStoryChapterTreeEditor::StoryDialogueTreeMode)
	{

		ToolbarBuilder.BeginSection("TransitionAction");
		{
			const FText NewCommonActionLabel = LOCTEXT("NewAction_Label", "New Common Action");
			const FText NewCommonActionTooltip = LOCTEXT("NewAction_ToolTip", "Create a new Common Action Blueprint from a base class");
			const FSlateIcon NewCommonActionIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "BTEditor.Graph.NewTask");

			ToolbarBuilder.AddToolBarButton(
				FUIAction(
					FExecuteAction::CreateSP(StoryEditorPtr.Get(), &FStoryChapterTreeEditor::CreateNewCommonAction),
					FCanExecuteAction::CreateSP(StoryEditorPtr.Get(), &FStoryChapterTreeEditor::CanCreateNewCommonAction),
					FIsActionChecked(),
					FIsActionButtonVisible::CreateSP(StoryEditorPtr.Get(), &FStoryChapterTreeEditor::IsNewCommonActionButtonVisible)
				),
				NAME_None,
				NewCommonActionLabel,
				NewCommonActionTooltip,
				NewCommonActionIcon
			);

			const FText NewSelectorActionLabel = LOCTEXT("NewAction_Label", "New Selector Action");
			const FText NewSelectorActionTooltip = LOCTEXT("NewAction_ToolTip", "Create a new Selector Action Blueprint from a base class");
			const FSlateIcon NewSelectorActionIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "BTEditor.Graph.NewTask");

			ToolbarBuilder.AddToolBarButton(
				FUIAction(
					FExecuteAction::CreateSP(StoryEditorPtr.Get(), &FStoryChapterTreeEditor::CreateNewSelectorAction),
					FCanExecuteAction::CreateSP(StoryEditorPtr.Get(), &FStoryChapterTreeEditor::CanCreateNewSelectorAction),
					FIsActionChecked(),
					FIsActionButtonVisible::CreateSP(StoryEditorPtr.Get(), &FStoryChapterTreeEditor::IsNewSelectorActionButtonVisible)
				),
				NAME_None,
				NewSelectorActionLabel,
				NewSelectorActionTooltip,
				NewSelectorActionIcon
			);
		}
		ToolbarBuilder.EndSection();
	}
}

#undef LOCTEXT_NAMESPACE
