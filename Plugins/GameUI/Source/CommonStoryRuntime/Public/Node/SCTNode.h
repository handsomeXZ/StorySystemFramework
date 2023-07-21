#pragma once

#include "CoreMinimal.h"
#include "Asset/StoryChapterTree.h"
#include "SCTNode.generated.h"

UCLASS(Abstract)
class USCTNode : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	
	/** @return tree asset */
	UStoryChapterTree* GetTreeAsset() const { return TreeAsset; }
	/** @return parent node */
	USCTNode* GetParentNode() const { return ParentNode; }




private:
	/** source asset */
	UPROPERTY()
	TObjectPtr<UStoryChapterTree> TreeAsset;

	/** parent node */
	UPROPERTY()
	TObjectPtr<USCTNode> ParentNode;

};