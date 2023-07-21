// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "Templates/SharedPointer.h"

class FUICommandInfo;

class FStoryEditorCommonCommands : public TCommands<FStoryEditorCommonCommands>
{
public:
	FStoryEditorCommonCommands();

	TSharedPtr<FUICommandInfo> NewDialogueTree;

	/** Initialize commands */
	virtual void RegisterCommands() override;
};

