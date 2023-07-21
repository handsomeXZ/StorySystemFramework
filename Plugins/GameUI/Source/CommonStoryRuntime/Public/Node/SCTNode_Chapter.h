#pragma once

#include "CoreMinimal.h"
#include "Node/SCTNode.h"
#include "Node/SCTNode_Transition.h"
#include "GameplayTagContainer.h"
#include "SCTNode_Chapter.generated.h"

class USCTNode_Chapter;


USTRUCT(BlueprintType)
struct FStoryDialogueTreeList
{
	GENERATED_BODY()
public:
	int32 ReadIndex = 0;

	UPROPERTY(EditAnywhere)
	TArray<UStoryDialogueTree*> SDTList;
};

USTRUCT()
struct FSCTCompositeChild
{
	GENERATED_BODY()
public:
	
	bool operator < (const FSCTCompositeChild& B) const
	{
		return ChildTransition->PriorityOrder > B.ChildTransition->PriorityOrder;
	}

	bool IsValid() {
		return ChildChapter != nullptr 
			&& ChildTransition != nullptr;
	}

	/** child node */
	UPROPERTY()
	TObjectPtr<USCTNode_Chapter> ChildChapter = nullptr;

	/** transition node */
	UPROPERTY()
	TObjectPtr<USCTNode_Transition> ChildTransition = nullptr;
};

UCLASS()
class COMMONSTORYRUNTIME_API USCTNode_Chapter : public USCTNode
{
	GENERATED_UCLASS_BODY()
public:

	/** find next child branch to execute */
	int32 FindChildToExecute() const;

	/** @return child node at given index */
	FSCTCompositeChild GetChildNode(int32 Index) const
	{
		return Children.IsValidIndex(Index) ? Children[Index] : FSCTCompositeChild();
	}

	/** @return children count */
	int32 GetChildrenNum() const { return Children.Num(); }

	void AddChildren(FSCTCompositeChild& Child);

public:
	UPROPERTY(EditAnywhere, Category = Chapter)
	FString ChapterName;
	UPROPERTY(EditAnywhere, Category = Chapter)
	FGameplayTag ChapterTag;

	UPROPERTY(EditAnywhere, Category = Chapter)
	TMap<FGameplayTag, FStoryDialogueTreeList> StoryDialogueTreeMap;

	/** child nodes */
	UPROPERTY()
	TArray<FSCTCompositeChild> Children;
};