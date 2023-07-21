// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework\GameStateBase.h"

#include "TYLGameState.generated.h"

class APlayerState;
class UAbilitySystemComponent;
class UExperienceManagerComponent;
class UObject;
struct FFrame;

/**
 * ATYLGameState
 *
 *	The base game state class used by this project.
 *	Pair this with a ATYLGameMode
 */
UCLASS(Config = Game)
class GAMEUI_API ATYLGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	ATYLGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface


private:
	UPROPERTY()
	TObjectPtr<UExperienceManagerComponent> ExperienceManagerComponent;


};
