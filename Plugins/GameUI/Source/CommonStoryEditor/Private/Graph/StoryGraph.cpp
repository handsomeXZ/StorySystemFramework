
#include "Graph/StoryGraph.h"

#include "SGraphNode.h"
#include "SGraphPanel.h"

#define LOCTEXT_NAMESPACE "StoryGraph"

/////////////////////////////////////////////////////
// UStoryGraph

UStoryGraph::UStoryGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAllowDeletion = false;
	bAllowRenaming = true;
	bLockUpdates = false;
}

void UStoryGraph::UpdateAsset_Internal(int32 UpdateFlags)
{
	if (bLockUpdates)
	{
		return;
	}

	UpdateAsset(UpdateFlags);
}

void UStoryGraph::OnCreated()
{
	// Initialize the graph
	const UEdGraphSchema* MySchema = GetSchema();
	MySchema->CreateDefaultNodesForGraph(*this);
}

void UStoryGraph::OnNodesPasted(const FString& ImportStr)
{
	// empty in base class
}

bool UStoryGraph::IsLocked() const
{
	return bLockUpdates;
}

void UStoryGraph::LockUpdates()
{
	bLockUpdates = true;
}

void UStoryGraph::UnlockUpdates()
{
	bLockUpdates = false;
	UpdateAsset_Internal();
}

void UStoryGraph::OnSave()
{
	UpdateAsset_Internal();
}

#undef LOCTEXT_NAMESPACE
