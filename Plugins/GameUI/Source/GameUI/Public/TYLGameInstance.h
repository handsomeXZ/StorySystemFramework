// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"

#include "TYLGameInstance.generated.h"

class ULocalPlayer;
struct FGameplayTag;

UCLASS(Config = Game)
class GAMEUI_API UTYLGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTYLGameInstance(const FObjectInitializer& ObjectInitializer);
	
	virtual int32 AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId) override;
	virtual bool RemoveLocalPlayer(ULocalPlayer* ExistingPlayer) override;
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void ReturnToMainMenu() override;

private:
	/** This is the primary player*/
	TWeakObjectPtr<ULocalPlayer> PrimaryPlayer;
};