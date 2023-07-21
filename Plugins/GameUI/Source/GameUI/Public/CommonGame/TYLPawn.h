// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularPawn.h"


#include "TYLPawn.generated.h"

/**
 * ATYLPawn
 */
UCLASS()
class GAMEUI_API ATYLPawn : public AModularPawn
{
	GENERATED_BODY()

public:

	ATYLPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayTag", meta = (Categories = "Gameplay.Role"))
	FGameplayTag IdentityTag;

	/**
	 * Adding a Pawn to the group Tag will force a response to the dialog Manager
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameplayTag", meta = (Categories = "Gameplay.StoryChapter"))
	TSet<FGameplayTag> DialogueGroupTag;

};
