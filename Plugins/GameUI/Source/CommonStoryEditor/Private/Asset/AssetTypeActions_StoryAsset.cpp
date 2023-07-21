#include "Asset/AssetTypeActions_StoryAsset.h"
#include "CommonStoryEditor.h"
#include "Asset/StoryChapterTree.h"
#include "Asset/StoryDialogueTree.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

//////////////////////////////////////////////////////////////////////////
// SCT

FText FAssetTypeActions_StoryChapterTree::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_StoryChapterTree", "Story Chapter Tree");
}

uint32 FAssetTypeActions_StoryChapterTree::GetCategories()
{
	FCommonStoryEditorModule& CSCModule = FModuleManager::LoadModuleChecked<FCommonStoryEditorModule>("CommonStoryEditor");
	return CSCModule.GetStoryChapterAssetCategoryBit();
}

UClass* FAssetTypeActions_StoryChapterTree::GetSupportedClass() const
{
	return UStoryChapterTree::StaticClass();
}

void FAssetTypeActions_StoryChapterTree::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (auto Object : InObjects)
	{
		UStoryChapterTree* StoryChapterTree = Cast<UStoryChapterTree>(Object);
		if (StoryChapterTree != nullptr)
		{
			FCommonStoryEditorModule& EditorModule = FModuleManager::GetModuleChecked<FCommonStoryEditorModule>("CommonStoryEditor");
			TSharedRef<FStoryChapterTreeEditor> NewEditor = EditorModule.CreateStoryChapterTreeEditor(Mode, EditWithinLevelEditor, StoryChapterTree);
			
		}
	}
}
//////////////////////////////////////////////////////////////////////////
// SDT

FText FAssetTypeActions_StoryDialogueTree::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_StoryDialogueTree", "Story Dialogue Tree");
}

uint32 FAssetTypeActions_StoryDialogueTree::GetCategories()
{
	FCommonStoryEditorModule& CSCModule = FModuleManager::LoadModuleChecked<FCommonStoryEditorModule>("CommonStoryEditor");
	return CSCModule.GetStoryChapterAssetCategoryBit();
}

UClass* FAssetTypeActions_StoryDialogueTree::GetSupportedClass() const
{
	return UStoryDialogueTree::StaticClass();
}

void FAssetTypeActions_StoryDialogueTree::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (auto Object : InObjects)
	{
		UStoryDialogueTree* StoryDialogueTree = Cast<UStoryDialogueTree>(Object);
		if (StoryDialogueTree != nullptr)
		{
			FCommonStoryEditorModule& EditorModule = FModuleManager::GetModuleChecked<FCommonStoryEditorModule>("CommonStoryEditor");
			TSharedRef<FStoryChapterTreeEditor> NewEditor = EditorModule.CreateStoryChapterTreeEditor(Mode, EditWithinLevelEditor, StoryDialogueTree);

		}
	}
}



#undef LOCTEXT_NAMESPACE