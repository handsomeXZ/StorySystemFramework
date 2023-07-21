// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Delegates/Delegate.h"
#include "EdGraph/EdGraph.h"
#include "Internationalization/Text.h"
#include "Misc/Attribute.h"
#include "Templates/SharedPointer.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class SDockTab;
class SGraphEditor;
class SWidget;
struct FSlateBrush;

struct FStoryChapterTreeEditorSummoner : public FWorkflowTabFactory
{
public:
	FStoryChapterTreeEditorSummoner(TSharedPtr<class FStoryChapterTreeEditor> InStoryChapterTreeEditorPtr);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

protected:
	TWeakPtr<class FStoryChapterTreeEditor> SCTEditorPtr;
};

//////////////////////////////////////////////////////////////////////////

struct FStoryChapterTreeDetailsSummoner : public FWorkflowTabFactory
{
public:
	FStoryChapterTreeDetailsSummoner(TSharedPtr<class FStoryChapterTreeEditor> InStoryChapterTreeEditorPtr);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

protected:
	TWeakPtr<class FStoryChapterTreeEditor> SCTEditorPtr;
};

//////////////////////////////////////////////////////////////////////////

struct FStoryDialogueTreeEditorSummoner : public FWorkflowTabFactory
{
public:
	FStoryDialogueTreeEditorSummoner(TSharedPtr<class FStoryChapterTreeEditor> InStoryChapterTreeEditorPtr);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

protected:
	TWeakPtr<class FStoryChapterTreeEditor> SCTEditorPtr;
};

//////////////////////////////////////////////////////////////////////////

struct FStoryDialogueTreeDetailsSummoner : public FWorkflowTabFactory
{
public:
	FStoryDialogueTreeDetailsSummoner(TSharedPtr<class FStoryChapterTreeEditor> InStoryChapterTreeEditorPtr);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

protected:
	TWeakPtr<class FStoryChapterTreeEditor> SCTEditorPtr;
};