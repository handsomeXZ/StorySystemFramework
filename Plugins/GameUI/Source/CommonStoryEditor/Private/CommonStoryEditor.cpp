// Copyright Epic Games, Inc. All Rights Reserved.

#include "CommonStoryEditor.h"

#include "Asset/AssetTypeActions_StoryAsset.h"
#include "Factory/StoryGraphFactory.h"
#include "Editor/StoryChapterTreeEditorToolkit.h"
#include "Asset/StoryDialogueTree.h"

#include "AIGraphTypes.h"

#include "AssetToolsModule.h"

#define LOCTEXT_NAMESPACE "FCommonStoryEditorModule"

const FName FCommonStoryEditorModule::StoryEditorAppIdentifier(TEXT("StoryEditorApp"));


void FCommonStoryEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	StoryGraphNodeFactory = MakeShareable(new FStoryNodeFactory());
	FEdGraphUtilities::RegisterVisualNodeFactory(StoryGraphNodeFactory);

	StoryPinConnectionFactory = MakeShareable(new FStoryPinConnectionFactory());
	FEdGraphUtilities::RegisterVisualPinConnectionFactory(StoryPinConnectionFactory);

	// Register asset types
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	StoryChapterAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("StoryAsset")), LOCTEXT("StoryChapterAssetCategory", "Story Asset"));

	const TSharedPtr<FAssetTypeActions_StoryChapterTree> SCTAssetTypeAction = MakeShareable(new FAssetTypeActions_StoryChapterTree);
	ItemDataAssetTypeActions.Add(SCTAssetTypeAction);
	AssetTools.RegisterAssetTypeActions(SCTAssetTypeAction.ToSharedRef());

	const TSharedPtr<FAssetTypeActions_StoryDialogueTree> SDTAssetTypeAction = MakeShareable(new FAssetTypeActions_StoryDialogueTree);
	ItemDataAssetTypeActions.Add(SDTAssetTypeAction);
	AssetTools.RegisterAssetTypeActions(SDTAssetTypeAction.ToSharedRef());

}

void FCommonStoryEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
		
	ClassCache.Reset();

	if (StoryGraphNodeFactory.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(StoryGraphNodeFactory);
		StoryGraphNodeFactory.Reset();
	}

	if (StoryPinConnectionFactory.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualPinConnectionFactory(StoryPinConnectionFactory);
		StoryPinConnectionFactory.Reset();
	}

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (auto& AssetTypeAction : ItemDataAssetTypeActions)
		{
			if (AssetTypeAction.IsValid())
			{
				AssetToolsModule.UnregisterAssetTypeActions(AssetTypeAction.ToSharedRef());
			}
		}
	}
	ItemDataAssetTypeActions.Empty();

} 

TSharedRef<FStoryChapterTreeEditor> FCommonStoryEditorModule::CreateStoryChapterTreeEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UObject* Object)
{
	if (!ClassCache.IsValid())
	{
		ClassCache = MakeShareable(new FGraphNodeClassHelper(USDTDActionBase::StaticClass()));
		ClassCache->UpdateAvailableBlueprintClasses();
	}
	TSharedRef<FStoryChapterTreeEditor> NewStoryChapterTreeEditor(new FStoryChapterTreeEditor());
	NewStoryChapterTreeEditor->InitializeAssetEditor(Mode, InitToolkitHost, Object);
	return NewStoryChapterTreeEditor;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCommonStoryEditorModule, CommonStoryEditor)