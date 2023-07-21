// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonGame/CommonDialogue_WidgetComponent.h"
#include "CommonGame/CommonDialogue_Interface.h"
#include "Manager/StoryDialogueTypes.h"
#include "TimerManager.h"

void UCommonDialogue_WidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	if (UUserWidget* widget = GetWidget())
	{
		widget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCommonDialogue_WidgetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// ...
	Super::EndPlay(EndPlayReason);
}

void UCommonDialogue_WidgetComponent::ShowTemporarily(FStoryDialogueContext& Context)
{
	if (UUserWidget* widget = GetWidget())
	{
		ICommonDialogue_Interface*  interfaceptr = Cast<ICommonDialogue_Interface>(GetWidget());
		check(interfaceptr);
		interfaceptr->SetDialogueToShow(Context);
		widget->SetVisibility(ESlateVisibility::Visible);
	}

}

void UCommonDialogue_WidgetComponent::Immediate_DiscardShowing()
{
	if (UUserWidget* widget = GetWidget())
	{
		widget->SetVisibility(ESlateVisibility::Hidden);
	}
}