// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonGame/TYLPawn.h"
#include "NativeGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TYLPawn)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_DEFAULT_ROLE, "Gameplay.Role.Default");

ATYLPawn::ATYLPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IdentityTag = TAG_DEFAULT_ROLE;
}


void ATYLPawn::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ATYLPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
