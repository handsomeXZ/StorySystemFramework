// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CommonDialogue_WidgetComponent.generated.h"

struct FStoryDialogueContext;

UCLASS()
class COMMONSTORYRUNTIME_API UCommonDialogue_WidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	// UActorComponent Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End UActorComponent Interface

	void ShowTemporarily(FStoryDialogueContext& Context);

	void Immediate_DiscardShowing();

	void OnNotify();
private:
	FTimerHandle TimerHanle;
};
