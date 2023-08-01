// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonGame/DialogueAction_ActionBase.h"
#include "DialogueAction_RuleAction.generated.h"

struct FStoryDialogueContext;

UCLASS(Abstract, Blueprintable, EditInlineNew, BlueprintType, meta = (DisplayName = "Dialogue Rule Action"))
class UDialogueAction_RuleAction : public UDialogueAction_ActionBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ExecuteAction"))
	void ExecuteAction_BlueprintImplement(FStoryDialogueContext Context, bool& RlueActionResult) const;

	void ExecuteAction(FStoryDialogueContext& Context, bool& RlueActionResult) const;
};
