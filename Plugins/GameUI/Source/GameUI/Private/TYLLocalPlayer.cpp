// Copyright Epic Games, Inc. All Rights Reserved.

#include "TYLLocalPlayer.h"

#include "GameFramework/PlayerController.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(TYLLocalPlayer)

void UTYLLocalPlayer::PostInitProperties()
{
	Super::PostInitProperties();
}


bool UTYLLocalPlayer::SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld)
{
	const bool bResult = Super::SpawnPlayActor(URL, OutError, InWorld);

	return bResult;
}

