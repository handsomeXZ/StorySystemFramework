
#include "Graph/SCTGraph.h"

#include "Node/SCTGraphNode_Chapter.h"
#include "Node/SCTGraphNode_Transition.h"
#include "Node/SCTGraphNode_Entry.h"
#include "Node/SCTNode_Chapter.h"
#include "Node/SCTNode_Transition.h"
#include "Asset/StoryChapterTree.h"

#include "SGraphNode.h"
#include "SGraphPanel.h"

#define LOCTEXT_NAMESPACE "SCTGraph"

struct FCompareNodeXLocation
{
	bool operator()(const UEdGraphPin& A, const UEdGraphPin& B) const
	{
		const UEdGraphNode* NodeA = A.GetOwningNode();
		const UEdGraphNode* NodeB = B.GetOwningNode();

		if (NodeA->NodePosX == NodeB->NodePosX)
		{
			return NodeA->NodePosY < NodeB->NodePosY;
		}

		return NodeA->NodePosX < NodeB->NodePosX;
	}
};

namespace SCTAutoArrangeHelpers
{
	struct FNodeBoundsInfo
	{
		FVector2D SubGraphBBox;
		TArray<FNodeBoundsInfo> Children;
	};

	UEdGraphPin* FindGraphNodePin(UEdGraphNode* Node, EEdGraphPinDirection Dir)
	{
		UEdGraphPin* Pin = nullptr;
		for (int32 Idx = 0; Idx < Node->Pins.Num(); Idx++)
		{
			if (Node->Pins[Idx]->Direction == Dir)
			{
				Pin = Node->Pins[Idx];
				break;
			}
		}

		return Pin;
	}

	void AutoArrangeNodes(USCTGraphNode* ParentNode, FNodeBoundsInfo& BBoxTree, float PosX, float PosY)
	{
		int32 BBoxIndex = 0;

		UEdGraphPin* Pin = FindGraphNodePin(ParentNode, EGPD_Output);
		if (Pin)
		{
			SGraphNode::FNodeSet NodeFilter;
			TArray<UEdGraphPin*> TempLinkedTo = Pin->LinkedTo;
			for (int32 Idx = 0; Idx < TempLinkedTo.Num(); Idx++)
			{
				USCTGraphNode* GraphNode = Cast<USCTGraphNode>(TempLinkedTo[Idx]->GetOwningNode());
				if (GraphNode && BBoxTree.Children.Num() > 0)
				{
					AutoArrangeNodes(GraphNode, BBoxTree.Children[BBoxIndex], PosX, PosY + GraphNode->DEPRECATED_NodeWidget.Pin()->GetDesiredSize().Y * 2.5f);
					GraphNode->DEPRECATED_NodeWidget.Pin()->MoveTo(FVector2D(BBoxTree.Children[BBoxIndex].SubGraphBBox.X / 2 - GraphNode->DEPRECATED_NodeWidget.Pin()->GetDesiredSize().X / 2 + PosX, PosY), NodeFilter);
					PosX += BBoxTree.Children[BBoxIndex].SubGraphBBox.X + 20;
					BBoxIndex++;
				}

			}
		}
	}

	void GetNodeSizeInfo(USCTGraphNode* ParentNode, FNodeBoundsInfo& BBoxTree)
	{
		BBoxTree.SubGraphBBox = ParentNode->DEPRECATED_NodeWidget.Pin()->GetDesiredSize();
		float LevelWidth = 0;
		float LevelHeight = 0;

		UEdGraphPin* Pin = FindGraphNodePin(ParentNode, EGPD_Output);
		if (Pin)
		{
			Pin->LinkedTo.Sort(FCompareNodeXLocation());
			for (int32 Idx = 0; Idx < Pin->LinkedTo.Num(); Idx++)
			{
				USCTGraphNode* GraphNode = Cast<USCTGraphNode>(Pin->LinkedTo[Idx]->GetOwningNode());
				if (GraphNode)
				{
					const int32 ChildIdx = BBoxTree.Children.Add(FNodeBoundsInfo());
					FNodeBoundsInfo& ChildBounds = BBoxTree.Children[ChildIdx];

					GetNodeSizeInfo(GraphNode, ChildBounds);

					LevelWidth += ChildBounds.SubGraphBBox.X + 20;
					if (ChildBounds.SubGraphBBox.Y > LevelHeight)
					{
						LevelHeight = ChildBounds.SubGraphBBox.Y;
					}
				}
			}

			if (LevelWidth > BBoxTree.SubGraphBBox.X)
			{
				BBoxTree.SubGraphBBox.X = LevelWidth;
			}

			BBoxTree.SubGraphBBox.Y += LevelHeight;
		}
	}
}

/////////////////////////////////////////////////////
// USCTGraph

USCTGraph::USCTGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAllowDeletion = false;
	bAllowRenaming = true;
}

void USCTGraph::UpdateAsset(int32 UpdateFlags)
{
	// we can't look at pins until pin references have been fixed up post undo:
	UEdGraphPin::ResolveAllPinReferences();
	if (Nodes.Num() > 0 && Nodes[0] && Nodes[0]->Pins.Num() > 0 && Nodes[0]->Pins[0]->LinkedTo.Num() > 0)
	{
		USCTGraphNode* Node = Cast<USCTGraphNode>(Nodes[0]->Pins[0]->LinkedTo[0]->GetOwningNode());
		CreateSCT(Node);
	}

}

void USCTGraph::CreateSCT(USCTGraphNode* RootGraphNode)
{
	UStoryChapterTree* SCTAsset = Cast<UStoryChapterTree>(GetOuter());
	SCTAsset->RootNode = nullptr; //discard old tree

	// Initialize Root Node
	SCTAsset->RootNode = Cast<USCTNode_Chapter>(RootGraphNode->NodeInstance);
	
	// Connect Tree Nodes
	ConnectSCTNode(SCTAsset, SCTAsset->RootNode, RootGraphNode);


	// Now remove any orphaned nodes left behind after regeneration
	RemoveOrphanedNodes();
}

void USCTGraph::CollectAllNodeInstances(TSet<UObject*>& NodeInstances)
{
	for (int32 Idx = 0; Idx < Nodes.Num(); Idx++)
	{
		USCTGraphNode* MyNode = Cast<USCTGraphNode>(Nodes[Idx]);
		if (MyNode)
		{
			NodeInstances.Add(MyNode->NodeInstance);
		}
	}
}

void USCTGraph::RemoveOrphanedNodes()
{
	TSet<UObject*> NodeInstances;
	CollectAllNodeInstances(NodeInstances);

	NodeInstances.Remove(nullptr);

	// Obtain a list of all nodes actually in the asset and discard unused nodes
	TArray<UObject*> AllInners;
	const bool bIncludeNestedObjects = false;
	GetObjectsWithOuter(GetOuter(), AllInners, bIncludeNestedObjects);
	for (auto InnerIt = AllInners.CreateConstIterator(); InnerIt; ++InnerIt)
	{
		UObject* TestObject = *InnerIt;
		if (!NodeInstances.Contains(TestObject) && CanRemoveNestedObject(TestObject))
		{
			TestObject->SetFlags(RF_Transient);
			TestObject->Rename(nullptr, GetTransientPackage(), REN_DontCreateRedirectors | REN_NonTransactional | REN_ForceNoResetLoaders);
		}
	}
}

bool USCTGraph::CanRemoveNestedObject(UObject* TestObject) const
{
	return !TestObject->IsA(UEdGraphNode::StaticClass()) &&
		!TestObject->IsA(UEdGraph::StaticClass()) &&
		!TestObject->IsA(UEdGraphSchema::StaticClass());
}

void USCTGraph::ConnectSCTNode(UStoryChapterTree* SCTAsset, USCTNode_Chapter* RootNode, USCTGraphNode* RootGraphNode)
{
	if (RootGraphNode == nullptr)
	{
		return;
	}
	
	RootNode->Children.Reset();

	// gather all nodes
	int32 ChildIdx = 0;
	for (int32 PinIdx = 0; PinIdx < RootGraphNode->Pins.Num(); PinIdx++)
	{
		UEdGraphPin* Pin = RootGraphNode->Pins[PinIdx];
		if (Pin->Direction != EGPD_Output)
		{
			continue;
		}

		// sort connections so that they're organized the same as user can see in the editor
		Pin->LinkedTo.Sort(FCompareNodeXLocation());

		for (int32 Index = 0; Index < Pin->LinkedTo.Num(); ++Index)
		{
			USCTGraphNode_Transition* TransitionGraphNode = Cast<USCTGraphNode_Transition>(Pin->LinkedTo[Index]->GetOwningNode());
			if (TransitionGraphNode == nullptr)
			{
				continue;
			}
			USCTGraphNode_Chapter* ChapterGraphNode = Cast<USCTGraphNode_Chapter>(TransitionGraphNode->GetOutputPin()->LinkedTo[0]->GetOwningNode());
			if (ChapterGraphNode == nullptr)
			{
				continue;
			}
			
			USCTNode_Transition* TransitionInstance = Cast<USCTNode_Transition>(TransitionGraphNode->NodeInstance);
			if (TransitionInstance && Cast<UStoryChapterTree>(TransitionInstance->GetOuter()) == nullptr)
			{
				TransitionInstance->Rename(nullptr, SCTAsset);
			}

			USCTNode_Chapter* ChapterInstance = Cast<USCTNode_Chapter>(ChapterGraphNode->NodeInstance);
			if (ChapterInstance && Cast<UStoryChapterTree>(ChapterInstance->GetOuter()) == nullptr)
			{
				ChapterInstance->Rename(nullptr, SCTAsset);
			}

			

			if (ChapterInstance == nullptr && TransitionInstance == nullptr)
			{
				continue;
			}

			// store child data
			FSCTCompositeChild& ChildInfo = RootNode->Children.AddDefaulted_GetRef();
			ChildInfo.ChildChapter = ChapterInstance;
			ChildInfo.ChildTransition = TransitionInstance;

			RootNode->Children.Sort();

			ConnectSCTNode(SCTAsset, ChapterInstance, ChapterGraphNode);
		}
	}
}

void USCTGraph::AutoArrange()
{
	USCTGraphNode* RootNode = nullptr;
	for (int32 Idx = 0; Idx < Nodes.Num(); Idx++)
	{
		RootNode = Cast<USCTGraphNode_Entry>(Nodes[Idx]);
		if (RootNode)
		{
			break;
		}
	}

	if (!RootNode)
	{
		return;
	}

	SCTAutoArrangeHelpers::FNodeBoundsInfo BBoxTree;
	SCTAutoArrangeHelpers::GetNodeSizeInfo(RootNode, BBoxTree);
	SCTAutoArrangeHelpers::AutoArrangeNodes(RootNode, BBoxTree, 0, RootNode->DEPRECATED_NodeWidget.Pin()->GetDesiredSize().Y * 2.5f);

	RootNode->NodePosX = BBoxTree.SubGraphBBox.X / 2 - RootNode->DEPRECATED_NodeWidget.Pin()->GetDesiredSize().X / 2;
	RootNode->NodePosY = 0;

	RootNode->DEPRECATED_NodeWidget.Pin()->GetOwnerPanel()->ZoomToFit(/*bOnlySelection=*/ false);
}

#undef LOCTEXT_NAMESPACE
