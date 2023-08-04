// Copyright Epic Games, Inc. All Rights Reserved.

#include "TYLGameInstance.h"

#include "CommonLocalPlayer.h"
#include "Delegates/Delegate.h"
#include "Engine/LocalPlayer.h"
#include "GameUIManagerSubsystem.h"
#include "GameplayTagContainer.h"
#include "ICommonUIModule.h"
#include "CommonUISettings.h"
#include "NativeGameplayTags.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(TYLGameInstance)

UTYLGameInstance::UTYLGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

int32 UTYLGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	int32 ReturnVal = Super::AddLocalPlayer(NewPlayer, UserId);
	if (ReturnVal != INDEX_NONE)
	{
		if (!PrimaryPlayer.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("AddLocalPlayer: Set %s to Primary Player"), *NewPlayer->GetName());
			PrimaryPlayer = NewPlayer;
		}
		
		GetSubsystem<UGameUIManagerSubsystem>()->NotifyPlayerAdded(Cast<UCommonLocalPlayer>(NewPlayer));
	}
	
	return ReturnVal;
}

bool UTYLGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
	if (PrimaryPlayer == ExistingPlayer)
	{
		//TODO: do we want to fall back to another player?
		PrimaryPlayer.Reset();
		UE_LOG(LogTemp, Log, TEXT("RemoveLocalPlayer: Unsetting Primary Player from %s"), *ExistingPlayer->GetName());
	}
	GetSubsystem<UGameUIManagerSubsystem>()->NotifyPlayerDestroyed(Cast<UCommonLocalPlayer>(ExistingPlayer));

	return Super::RemoveLocalPlayer(ExistingPlayer);
}

void UTYLGameInstance::Init()
{
	Super::Init();

	// After subsystems are initialized, hook them together
	//FGameplayTagContainer PlatformTraits = ICommonUIModule::GetSettings().GetPlatformTraits();

	//// Register our custom init states
	//UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);

	//if (ensure(ComponentManager))
	//{
	//	const FLyraGameplayTags& GameplayTags = FLyraGameplayTags::Get();

	//	ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
	//	ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
	//	ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
	//	ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);
	//}


}

void UTYLGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UTYLGameInstance::ReturnToMainMenu()
{
	Super::ReturnToMainMenu();
}
