// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettingsBackedByCVars.h"
#include "UObject/SoftObjectPath.h"
#include "Asset/StoryChapterTree.h"
#include "CommonStorySettings.generated.h"

class UObject;

/**
 * Settings for a Common Story.
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="Common Story Settings"))
class UCommonStorySettings : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()

public:
	UCommonStorySettings();

public:
	// The StoryChapterTree to load for the StoryChapterManager.
	UPROPERTY(config, EditAnywhere, Category=StoryChapter)
	TSoftObjectPtr<UStoryChapterTree> StoryChapterTree;

 	//UPROPERTY(config, EditAnywhere, Category=Configuration, meta=(ForceUnits=s, ConsoleVariable="CommonLoadingScreen.HoldLoadingScreenAdditionalSecs"))
	//float HoldLoadingScreenAdditionalSecs = 2.0f;

};

