// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Engine/Blueprint.h"
#include "StoryChapterTree.generated.h"


UCLASS(BlueprintType)
class COMMONSTORYRUNTIME_API UStoryChapterTree : public UObject
{
	GENERATED_UCLASS_BODY()

	/** root node of loaded tree */
	UPROPERTY()
	TObjectPtr<class USCTNode_Chapter> RootNode;

#if WITH_EDITORONLY_DATA

	/** Graph for Behavior Tree */
	UPROPERTY()
	TObjectPtr<class UEdGraph>	EdGraph;

	/** The StoryDialogueTree we last edited */
	UPROPERTY(EditAnywhere, Category="Default Editing")
	TObjectPtr<class UStoryDialogueTree> EditingSDT;

#endif

};
