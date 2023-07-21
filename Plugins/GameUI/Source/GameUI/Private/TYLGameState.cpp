// Copyright Epic Games, Inc. All Rights Reserved.

#include "TYLGameState.h"

#include "Components/GameFrameworkComponentManager.h"
#include "ExperienceManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TYLGameState)

ATYLGameState::ATYLGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.bStartWithTickEnabled = true;

	ExperienceManagerComponent = CreateDefaultSubobject<UExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));

}

void ATYLGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ATYLGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATYLGameState::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);

	Super::BeginPlay();
}

void ATYLGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);

	Super::EndPlay(EndPlayReason);
}

