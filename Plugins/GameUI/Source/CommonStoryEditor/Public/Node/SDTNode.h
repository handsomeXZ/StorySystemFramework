#pragma once

#include "CoreMinimal.h"
#include "Asset/StoryDialogueTree.h"
#include "SDTNode.generated.h"

UCLASS(Abstract)
class USDTNode : public UObject
{
	GENERATED_BODY()
public:
	USDTNode() : NodeType(ESDTNodeType::UnKnown) {}
	USDTNode(ESDTNodeType NodeTypeIn) : NodeType(NodeTypeIn) {}

	/** @return tree asset */
	UStoryDialogueTree* GetTreeAsset() const { return TreeAsset; }
	/** @return parent node */
	USDTNode* GetParentNode() const { return ParentNode; }


public:
	ESDTNodeType NodeType;

private:
	/** source asset */
	UPROPERTY()
	TObjectPtr<UStoryDialogueTree> TreeAsset;

	/** parent node */
	UPROPERTY()
	TObjectPtr<USDTNode> ParentNode;

};