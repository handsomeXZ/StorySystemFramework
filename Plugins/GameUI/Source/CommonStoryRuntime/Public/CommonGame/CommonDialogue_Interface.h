// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CommonDialogue_Interface.generated.h"

struct FStoryDialogueContext;

/**
 * 
 */
UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class COMMONSTORYRUNTIME_API UCommonDialogue_Interface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class COMMONSTORYRUNTIME_API ICommonDialogue_Interface : public IInterface
{
	GENERATED_IINTERFACE_BODY()
public:
	virtual void SetDialogueToShow(FStoryDialogueContext& Context){};
};