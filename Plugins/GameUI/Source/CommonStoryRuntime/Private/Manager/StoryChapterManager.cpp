#include "Manager/StoryChapterManager.h"

#include "Asset/StoryChapterTree.h"
#include "Node/SCTNode_Chapter.h"
#include "Setting\CommonStorySettings.h"

#include "NativeGameplayTags.h"


UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GAMEPLAY_STORYCHAPTER_DEFAULT, "Gameplay.StoryChapter.Default");

UStoryChapterManager::UStoryChapterManager()
{
	StoryChapterTree = nullptr;
	CurrentChapter = nullptr;
}

void UStoryChapterManager::Tick(float DeltaTime)
{
	Update();
}

TStatId UStoryChapterManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UStoryChapterManager, STATGROUP_Tickables);
}

ETickableTickType UStoryChapterManager::GetTickableTickType() const
{
	return (HasAnyFlags(RF_ClassDefaultObject))
		? ETickableTickType::Never
		: ETickableTickType::Always;
}

void UStoryChapterManager::Update()
{
	if (StoryChapterTree != nullptr && CurrentChapter != nullptr)
	{
		for (FSCTCompositeChild& child : CurrentChapter->Children)
		{
			if (child.ChildTransition->bAutomaticTransition || (child.ChildTransition->Action && child.ChildTransition->Action->Execute()))
			{
				CurrentChapter = child.ChildChapter;

				PrevStoryChapterTag = CurrentStoryChapterTag;
				CurrentStoryChapterTag = CurrentChapter->ChapterTag;
				
				return;
			}
		}
	}
}

void UStoryChapterManager::Initialize(FSubsystemCollectionBase& Collection)
{
	CurrentStoryChapterTag = TAG_GAMEPLAY_STORYCHAPTER_DEFAULT;
	PrevStoryChapterTag = TAG_GAMEPLAY_STORYCHAPTER_DEFAULT;

	const UCommonStorySettings* Settings = GetDefault<UCommonStorySettings>();
	StoryChapterTree = Settings->StoryChapterTree.LoadSynchronous();
}

void UStoryChapterManager::Deinitialize()
{
	// ...
}

void UStoryChapterManager::ActiveStory()
{
	if (StoryChapterTree)
	{
		CurrentChapter = StoryChapterTree->RootNode;
		CurrentStoryChapterTag = CurrentChapter->ChapterTag;
	}
}

FGameplayTag UStoryChapterManager::GetCurrentStoryChapterTag()
{
	if (CurrentChapter)
	{
		return CurrentChapter->ChapterTag;
	}

	return TAG_GAMEPLAY_STORYCHAPTER_DEFAULT;
}

FString UStoryChapterManager::GetCurrentStoryChapterName()
{
	if (CurrentChapter)
	{
		return CurrentChapter->ChapterName;
	}

	return FString();
}

UStoryDialogueTree* UStoryChapterManager::GetStoryDialogueTree(FGameplayTag IdentifyTag, bool bReadAdded)
{
	if (CurrentChapter)
	{
		if (FStoryDialogueTreeList* List = CurrentChapter->StoryDialogueTreeMap.Find(IdentifyTag))
		{
			if (List->ReadIndex < List->SDTList.Num())
			{
				UStoryDialogueTree* SDT = List->SDTList[List->ReadIndex];
				if (bReadAdded && List->ReadIndex + 1 < List->SDTList.Num())
				{
					List->ReadIndex++;
				}
				
				return SDT;
			}
		}
	}

	return nullptr;
}