// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "ControlFlowNode.h"
//#include "LoadingProcessInterface.h"

#include "TYLStateComponent.generated.h"

class FControlFlow;
class UCommonActivatableWidget;
class UExperienceDefinition;

UCLASS(Abstract)
class UTYLStateComponent : public UGameStateComponent/*, public ILoadingProcessInterface*/
{
	GENERATED_BODY()

public:

	UTYLStateComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface

	////~ILoadingProcessInterface interface
	//virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;
	////~End of ILoadingProcessInterface

private:
	void OnExperienceLoaded(const UExperienceDefinition* Experience);

	void FlowStep_TryShowPressStartScreen(FControlFlowNodeRef SubFlow);
	void FlowStep_TryShowMainScreen(FControlFlowNodeRef SubFlow);

	bool bShouldShowLoadingScreen = true;

	UPROPERTY(EditAnywhere, Category = UI)
	TSoftClassPtr<UCommonActivatableWidget> PressStartScreenClass;

	UPROPERTY(EditAnywhere, Category = UI)
	TSoftClassPtr<UCommonActivatableWidget> MainScreenClass;

	TSharedPtr<FControlFlow> FrontEndFlow;
	
	// If set, this is the in-progress press start screen task
	FControlFlowNodePtr InProgressPressStartScreen;

	FDelegateHandle OnJoinSessionCompleteEventHandle;
};
