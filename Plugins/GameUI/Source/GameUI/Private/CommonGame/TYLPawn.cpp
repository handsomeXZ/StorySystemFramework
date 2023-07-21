// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonGame/TYLPawn.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TYLPawn)


ATYLPawn::ATYLPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IdentityTag = FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Role.NPC.Default"));
}


void ATYLPawn::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ATYLPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
