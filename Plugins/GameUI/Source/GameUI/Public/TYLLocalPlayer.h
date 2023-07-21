// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonLocalPlayer.h"

#include "TYLLocalPlayer.generated.h"

class APlayerController;

/**
 * UTYLLocalPlayer
 */
UCLASS()
class GAMEUI_API UTYLLocalPlayer : public UCommonLocalPlayer
{
	GENERATED_BODY()

public:

	UTYLLocalPlayer();

	//~UObject interface
	virtual void PostInitProperties() override;
	//~End of UObject interface

	//~ULocalPlayer interface
	virtual bool SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld) override;
	//~End of ULocalPlayer interface
};
