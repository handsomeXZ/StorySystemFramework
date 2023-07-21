// Copyright Epic Games, Inc. All Rights Reserved.

#include "Editor/StoryEditorCommands.h"

#include "Framework/Commands/InputChord.h"
#include "Framework/Commands/UICommandInfo.h"
#include "GenericPlatform/GenericApplication.h"
#include "InputCoreTypes.h"
#include "Internationalization/Internationalization.h"
#include "Styling/AppStyle.h"
#include "UObject/NameTypes.h"
#include "UObject/UnrealNames.h"

#define LOCTEXT_NAMESPACE "StoryEditorCommands"

FStoryEditorCommonCommands::FStoryEditorCommonCommands()
	: TCommands<FStoryEditorCommonCommands>("StoryEditor.Common", LOCTEXT("StoryEditorCommandLabel", "StoryEditor"), NAME_None, FAppStyle::GetAppStyleSetName())
{
}

void FStoryEditorCommonCommands::RegisterCommands()
{
	UI_COMMAND(NewDialogueTree, "New Dialogue Tree", "Create a new Dialogue Tree Data Asset", EUserInterfaceActionType::Button, FInputChord());
}


#undef LOCTEXT_NAMESPACE
