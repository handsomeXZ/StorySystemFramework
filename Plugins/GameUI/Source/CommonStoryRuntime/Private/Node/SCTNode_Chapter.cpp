#include "Node/SCTNode_Chapter.h"



USCTNode_Chapter::USCTNode_Chapter(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
	, ChapterName(TEXT("Chapter"))
{
	
}
int32 USCTNode_Chapter::FindChildToExecute() const
{
	return 1;
}

void USCTNode_Chapter::AddChildren(FSCTCompositeChild& Child)
{
	struct FCompareTransitionsByPriority
	{
		FORCEINLINE bool operator()(const FSCTCompositeChild& A, const FSCTCompositeChild& B) const
		{
			return A.ChildTransition->PriorityOrder < B.ChildTransition->PriorityOrder;
		}
	};

	if (!Child.IsValid())
	{
		return;
	}

	Children.Add(Child);
	Children.Sort(FCompareTransitionsByPriority());
}