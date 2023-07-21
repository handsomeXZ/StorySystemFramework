
#pragma once

#include "CoreMinimal.h"
#include "Graph/StoryGraph.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "SDTGraph.generated.h"

class UObject;
class UStoryDialogueTree;
class USDTGraphNode;
class USDTNode;
struct FIndexHandle;

UCLASS(MinimalAPI)
class USDTGraph : public UStoryGraph
{
	GENERATED_UCLASS_BODY()

	virtual void UpdateAsset(int32 UpdateFlags = 0);

	void CreateSDT(USDTGraphNode* RootGraphNode);
	void ConnectSDTNode(UStoryDialogueTree* SCTAsset, USDTGraphNode* GraphNode, FIndexHandle ContinueToIndex, FIndexHandle ReturnToIndex, bool bExpectReturnNode = false);
	void RectifyConnection(UStoryDialogueTree* SDTAsset, USDTGraphNode* RootGraphNode, int32 OptionsNodeCount = 0);

	void RemoveOrphanedNodes();
	bool CanRemoveNestedObject(UObject* TestObject) const;
	void CollectAllNodeInstances(TSet<UObject*>& NodeInstances);

	// Parent instance node
	UPROPERTY()
	TObjectPtr<USDTGraphNode> OwnerSDTGraphNode;
};

