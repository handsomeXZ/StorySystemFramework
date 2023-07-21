// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "DialogueAction_ActionBase.generated.h"

/**
 * In fact, using 'DefaultToInstanced' should not effect.
 * since UClass is all initialized with CDO.
 */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class UDialogueAction_ActionBase : public UObject
{
	GENERATED_UCLASS_BODY()
	/**
	 * @TODO:
	 * Asynchronous support
	 */
public:
	void Initialize(const FWorldContext& WorldContext);

	virtual UWorld* GetWorld() const override final;
private:
	TObjectPtr<UWorld> CacheWorld;
};
