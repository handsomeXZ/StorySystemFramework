// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"

#include "TYLPlayerController.generated.h"



class APawn;
class APlayerState;
class FPrimitiveComponentId;
class IInputInterface;
class UObject;
class UPlayer;
struct FFrame;

/**
 * ATYLPlayerController
 *
 *	The base player controller class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base player controller class used by this project."))
class GAMEUI_API ATYLPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()

public:

	ATYLPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

	//~APlayerController interface
	virtual void ReceivedPlayer() override;
	//~End of APlayerController interface
	
private:
	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;
protected:
	//~APlayerController interface
	virtual void SetPlayer(UPlayer* InPlayer) override;

	virtual void UpdateForceFeedback(IInputInterface* InputInterface, const int32 ControllerId) override;
	virtual void UpdateHiddenComponents(const FVector& ViewLocation, TSet<FPrimitiveComponentId>& OutHiddenComponents) override;
	//~End of APlayerController interface

	bool bHideViewTargetPawnNextFrame = false;
};


// A player controller used for replay capture and playback
UCLASS()
class ATYLReplayPlayerController : public ATYLPlayerController
{
	GENERATED_BODY()

	virtual void SetPlayer(UPlayer* InPlayer) override;
};
