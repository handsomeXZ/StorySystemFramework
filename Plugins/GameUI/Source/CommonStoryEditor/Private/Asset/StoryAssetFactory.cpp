#include "Asset/StoryAssetFactory.h"

#include "Asset/StoryChapterTree.h"
#include "Asset/StoryDialogueTree.h"

#define LOCTEXT_NAMESPACE "StoryAssetFactory"

//////////////////////////////////////////////////////////////////////////
// SCT

UStoryChapterTreeFactory::UStoryChapterTreeFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UStoryChapterTree::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UStoryChapterTreeFactory::CanCreateNew() const
{
	return true;
}

UObject* UStoryChapterTreeFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(Class->IsChildOf(UStoryChapterTree::StaticClass()));
	return NewObject<UStoryChapterTree>(InParent, Class, Name, Flags);
}

//////////////////////////////////////////////////////////////////////////
// SDT

UStoryDialogueTreeFactory::UStoryDialogueTreeFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UStoryDialogueTree::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

bool UStoryDialogueTreeFactory::CanCreateNew() const
{
	return true;
}

UObject* UStoryDialogueTreeFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(Class->IsChildOf(UStoryDialogueTree::StaticClass()));
	return NewObject<UStoryDialogueTree>(InParent, Class, Name, Flags);
}

#undef LOCTEXT_NAMESPACE