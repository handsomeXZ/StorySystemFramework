// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonGame/TYLActivatableWidget.h"
#include "CommonDialogue_Interface.h"
#include "CommonPlayerDialogue_Widget.generated.h"

struct FStoryDialogueContext;

UCLASS(Abstract, Blueprintable)
class COMMONSTORYRUNTIME_API UCommonPlayerDialogue_Widget : public UTYLActivatableWidget, public ICommonDialogue_Interface
{
	GENERATED_BODY()
public:
	
	// ICommonDialogue_Interface
	virtual void SetDialogueToShow(FStoryDialogueContext& Context) override;
	// ~ End ICommonDialogue_Interface

	UFUNCTION(BlueprintImplementableEvent, Category = "DialogueWidget", meta = (DisplayName = "Set Dialogue Options To Show"))
	void BP_SetDialogueToShow(FStoryDialogueContext Context);
};
