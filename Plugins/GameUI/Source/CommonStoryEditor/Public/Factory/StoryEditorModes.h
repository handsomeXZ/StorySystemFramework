// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Templates/SharedPointer.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

/** Application mode for main Story Chapter Tree editing mode */
class FSCTEditorApplicationMode : public FApplicationMode
{
public:
	FSCTEditorApplicationMode(TSharedPtr<class FStoryChapterTreeEditor> InSCTEditor);

	virtual void RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

protected:
	TWeakPtr<class FStoryChapterTreeEditor> SCTEditor;

	// Set of spawnable tabs in Story Chapter Tree editing mode
	FWorkflowAllowedTabSet SCTTabFactories;
};

/** Application mode for Story Dialogue Tree editing mode */
class FSDTEditorApplicationMode : public FApplicationMode
{
public:
	FSDTEditorApplicationMode(TSharedPtr<class FStoryChapterTreeEditor> InSCTEditor);

	virtual void RegisterTabFactories(TSharedPtr<class FTabManager> InTabManager) override;
	virtual void PostActivateMode() override;

protected:
	TWeakPtr<class FStoryChapterTreeEditor> SCTEditor;

	// Set of spawnable tabs in Story Dialogue Tree mode
	FWorkflowAllowedTabSet SDTTabFactories;
};
