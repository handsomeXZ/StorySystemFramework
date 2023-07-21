// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "AssetTypeCategories.h"

class FStoryChapterTreeEditor;

class FCommonStoryEditorModule : public IModuleInterface
{
public:
	/** Story app identifier string */
	static const FName StoryEditorAppIdentifier;


	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/* Only virtual so that it can be called across the DLL boundary. */
	virtual TSharedRef<FStoryChapterTreeEditor> CreateStoryChapterTreeEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UObject* Object);

	EAssetTypeCategories::Type GetStoryChapterAssetCategoryBit() const { return StoryChapterAssetCategoryBit; }

	TSharedPtr<struct FGraphNodeClassHelper> GetClassCache() { return ClassCache; }

protected:
	EAssetTypeCategories::Type StoryChapterAssetCategoryBit;

private:

	/** Asset type actions */
	TArray<TSharedPtr<class FAssetTypeActions_Base>> ItemDataAssetTypeActions;
	TSharedPtr<class FStoryNodeFactory> StoryGraphNodeFactory;
	TSharedPtr<struct FStoryPinConnectionFactory> StoryPinConnectionFactory;

	TSharedPtr<struct FGraphNodeClassHelper> ClassCache;

};
