
#pragma once

#include "CoreMinimal.h"
#include "Graph/StoryGraph.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "SCTGraph.generated.h"

class UObject;
class UStoryChapterTree;
class USCTNode_Chapter;
class USCTGraphNode;

UCLASS(MinimalAPI)
class USCTGraph : public UStoryGraph
{
	GENERATED_UCLASS_BODY()

	virtual void UpdateAsset(int32 UpdateFlags = 0);

	void CreateSCT(USCTGraphNode* RootGraphNode);
	void ConnectSCTNode(UStoryChapterTree* SCTAsset, USCTNode_Chapter* RootNode, USCTGraphNode* GraphNode);

	void RemoveOrphanedNodes();
	bool CanRemoveNestedObject(UObject* TestObject) const;
	void CollectAllNodeInstances(TSet<UObject*>& NodeInstances);

	void AutoArrange();
};

