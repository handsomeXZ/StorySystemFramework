// Copyright Epic Games, Inc. All Rights Reserved.

#include "UIDemoGameMode.h"
#include "UIDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUIDemoGameMode::AUIDemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
