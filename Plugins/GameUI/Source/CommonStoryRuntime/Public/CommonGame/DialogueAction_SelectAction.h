// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonGame/DialogueAction_ActionBase.h"
#include "DialogueAction_SelectAction.generated.h"

struct FStoryDialogueContext;

UCLASS(Abstract, Blueprintable, BlueprintType, meta = (DisplayName = "Dialogue Select Action"))
class UDialogueAction_SelectAction : public UDialogueAction_ActionBase
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ExecuteAction"))
	void ExecuteAction_BlueprintImplement(FStoryDialogueContext Context) const;

	void ExecuteAction(FStoryDialogueContext& Context) const;

};
