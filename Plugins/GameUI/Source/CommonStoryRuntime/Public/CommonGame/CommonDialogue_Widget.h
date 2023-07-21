// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonDialogue_Interface.h"
#include "Blueprint/UserWidget.h"
#include "CommonDialogue_Widget.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class COMMONSTORYRUNTIME_API UCommonDialogue_Widget : public UUserWidget, public ICommonDialogue_Interface
{
	GENERATED_BODY()
public:
	// ICommonDialogue_Interface
	virtual void SetDialogueToShow(FStoryDialogueContext& Context) override;
	// ~ End ICommonDialogue_Interface
	UFUNCTION(BlueprintImplementableEvent, Category = DialogueWidget, meta = (DisplayName = "Set Dialogue Content To Show"))
	void BP_SetDialogueToShow(FStoryDialogueContext Context);
};
