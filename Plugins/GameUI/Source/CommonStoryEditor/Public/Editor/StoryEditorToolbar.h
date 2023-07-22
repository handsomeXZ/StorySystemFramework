// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Templates/SharedPointer.h"

class FStoryChapterTreeEditor;
class FExtender;
class FToolBarBuilder;

class FStoryEditorToolbar : public TSharedFromThis<FStoryEditorToolbar>
{
public:
	FStoryEditorToolbar(TSharedPtr<FStoryChapterTreeEditor> InStoryEditor)
		: StoryEditor(InStoryEditor) {}

	void AddModesToolbar(TSharedPtr<FExtender> Extender);
	void AddStoryChapterTreeToolbar(TSharedPtr<FExtender> Extender);
	void AddStoryDialogueTreeToolbar(TSharedPtr<FExtender> Extender);
private:
	void FillModesToolbar(FToolBarBuilder& ToolbarBuilder);
	void FillStoryChapterTreeToolbar(FToolBarBuilder& ToolbarBuilder);
	void FillStoryDialogueTreeToolbar(FToolBarBuilder& ToolbarBuilder);

protected:
	/** Pointer back to the blueprint editor tool that owns us */
	TWeakPtr<FStoryChapterTreeEditor> StoryEditor;
};
