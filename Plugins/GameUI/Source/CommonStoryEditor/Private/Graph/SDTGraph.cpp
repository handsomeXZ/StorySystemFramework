
#include "Graph/SDTGraph.h"

#include "Asset/StoryDialogueTree.h"
#include "Node/SDTGraphNode.h"
#include "Node/SDTGraphNode_DContent.h"
#include "Node/SDTGraphNode_DSelector.h"
#include "Node/SDTGraphNode_DOptions.h"
#include "Node/SDTGraphNode_DReturn.h"
#include "Node/SDTGraphNode_DExit.h"
#include "Node/SDTGraphNode_DContinue.h"
#include "Node/SDTGraphNode_DEntry.h"
#include "Node/SDTGraphNode_DAction.h"
#include "Node/SDTNode.h"
#include "Node/SDTNode_DContent.h"
#include "Node/SDTNode_DOptions.h"
#include "Node/SDTNode_DAction_Instance.h"
#include "Node/SDTNode_DAction_UnInstance.h"
#include "Node/SDTNode_DSelector_Instance.h"
#include "Node/SDTNode_DSelector_UnInstance.h"
#include "Node/SDTNode_DReturn.h"
#include "Node/SDTNode_DContinue.h"
#include "Node/SDTNode_DExit.h"
#include "SchemaAction/SDTGraphEdSchemaActions.h"

#include "SGraphNode.h"
#include "SGraphPanel.h"

#define LOCTEXT_NAMESPACE "SDTGraph"


struct FCompareNodeYLocation
{
	bool operator()(const UEdGraphPin& A, const UEdGraphPin& B) const
	{
		const UEdGraphNode* NodeA = A.GetOwningNode();
		const UEdGraphNode* NodeB = B.GetOwningNode();

		if (NodeA->NodePosY == NodeB->NodePosY)
		{
			return NodeA->NodePosX < NodeB->NodePosX;
		}

		return NodeA->NodePosY < NodeB->NodePosY;
	}
};

/////////////////////////////////////////////////////
// USDTGraph

USDTGraph::USDTGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAllowDeletion = false;
	bAllowRenaming = true;
}

void USDTGraph::UpdateAsset(int32 UpdateFlags)
{
	// we can't look at pins until pin references have been fixed up post undo:
	UEdGraphPin::ResolveAllPinReferences();
	if (Nodes.Num() > 0 && Nodes[0] && Nodes[0]->Pins.Num() > 0 && Nodes[0]->Pins[0]->LinkedTo.Num() > 0)
	{
		USDTGraphNode* Node = Cast<USDTGraphNode>(Nodes[0]->Pins[0]->LinkedTo[0]->GetOwningNode());
		CreateSDT(Node);
	}

}

void USDTGraph::CreateSDT(USDTGraphNode* RootGraphNode)
{
	UStoryDialogueTree* SDTAsset = Cast<UStoryDialogueTree>(GetOuter());

	// Initialize
	SDTAsset->Empty();
	FIndexHandle RootIndexHanle;
	RootIndexHanle.NodeType = Cast<USDTNode>(RootGraphNode->NodeInstance)->NodeType;
	RootIndexHanle.Index = 0;
	SDTAsset->RootIndex = RootIndexHanle;
	
	// Rectify Tree
	RectifyConnection(SDTAsset, RootGraphNode);

	// Connect Tree Nodes
	FIndexHandle Null_IndexHandle;
	Null_IndexHandle.Index = -1;
	Null_IndexHandle.NodeType = ESDTNodeType::UnKnown;
	ConnectSDTNode(SDTAsset, RootGraphNode, Null_IndexHandle, Null_IndexHandle);

	// Now remove any orphaned nodes left behind after regeneration
	RemoveOrphanedNodes();
}

void USDTGraph::CollectAllNodeInstances(TSet<UObject*>& NodeInstances)
{
	for (int32 Idx = 0; Idx < Nodes.Num(); Idx++)
	{
		USDTGraphNode* MyNode = Cast<USDTGraphNode>(Nodes[Idx]);
		if (MyNode)
		{
			NodeInstances.Add(MyNode->NodeInstance);
		}
	}
}

void USDTGraph::RemoveOrphanedNodes()
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

bool USDTGraph::CanRemoveNestedObject(UObject* TestObject) const
{
	return !TestObject->IsA(UEdGraphNode::StaticClass()) &&
		!TestObject->IsA(UEdGraph::StaticClass()) &&
		!TestObject->IsA(UEdGraphSchema::StaticClass());
}

void USDTGraph::RectifyConnection(UStoryDialogueTree* SDTAsset, USDTGraphNode* RootGraphNode, int32 OptionsNodeCount)
{
	if (RootGraphNode == nullptr)
	{
		return;
	}

	/*RootNode->Children.Reset();*/

	if (USDTGraphNode_DExit* SDTNode = Cast<USDTGraphNode_DExit>(RootGraphNode))
	{
		// ...
		return;
	}

	if (USDTGraphNode_DOptions* SDTGraphNode = Cast<USDTGraphNode_DOptions>(RootGraphNode))
	{
		OptionsNodeCount++;
	}

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
		//Pin->LinkedTo.Sort(FCompareNodeYLocation());

		for (int32 Index = 0; Index < Pin->LinkedTo.Num(); ++Index)
		{
			USDTGraphNode* MyNode = Cast<USDTGraphNode>(Pin->LinkedTo[Index]->GetOwningNode());

			RectifyConnection(SDTAsset, MyNode, OptionsNodeCount);
		}

		if (Pin->LinkedTo.Num() <= 0)
		{
			if (OptionsNodeCount >= 2)
			{
				USDTGraphNode_DReturn* NodeTemplate = NewObject<USDTGraphNode_DReturn>(SDTAsset->EdGraph, USDTGraphNode_DReturn::StaticClass());
				NodeTemplate->NodeClass = USDTNode_DReturn::StaticClass();

				USDTGraphNode_DReturn* ReturnNode = FSDTSchemaAction_NewNode::SpawnNodeFromTemplate(SDTAsset->EdGraph, NodeTemplate, FVector2D(RootGraphNode->NodePosX + 200, RootGraphNode->NodePosY));

				SDTAsset->EdGraph->GetSchema()->TryCreateConnection(RootGraphNode->Pins[PinIdx], ReturnNode->Pins[0]);
			}
			else
			{
				/**
				* @TODO
				* Nodes[1] is Exit Node.
				* This type of hard coding should not be used.
				*/
				SDTAsset->EdGraph->GetSchema()->TryCreateConnection(RootGraphNode->Pins[PinIdx], Nodes[1]->Pins[0]);
			}
		}
	}
}

void USDTGraph::ConnectSDTNode(UStoryDialogueTree* SDTAsset, USDTGraphNode* RootGraphNode, FIndexHandle ContinueToIndex, FIndexHandle ReturnToIndex, bool bExpectReturnNode)
{
	if (RootGraphNode == nullptr)
	{
		return;
	}
	
	/*RootNode->Children.Reset();*/

	if (USDTGraphNode_DReturn* RootReturnNode = Cast<USDTGraphNode_DReturn>(RootGraphNode))
	{
		// Return
		FSDTDReturnNode& NewNode = SDTAsset->ReturnNodeList.AddDefaulted_GetRef();
		
		FIndexHandle CurrentIndexHanle;
		CurrentIndexHanle.NodeType = Cast<USDTNode>(RootReturnNode->NodeInstance)->NodeType;
		CurrentIndexHanle.Index = SDTAsset->GetNodeListNum(CurrentIndexHanle.NodeType) - 1;

		NewNode.NodeIndex = CurrentIndexHanle;

		// Child
		if (bExpectReturnNode && ReturnToIndex.Index != -1)
		{
			NewNode.Child = ReturnToIndex;
		}

		return;
	}
	else if (USDTGraphNode_DContinue* RootContinueNode = Cast<USDTGraphNode_DContinue>(RootGraphNode))
	{
		// Continue
		FSDTDReturnNode& NewNode = SDTAsset->ReturnNodeList.AddDefaulted_GetRef();
		
		FIndexHandle CurrentIndexHanle;
		CurrentIndexHanle.NodeType = Cast<USDTNode>(RootContinueNode->NodeInstance)->NodeType;
		CurrentIndexHanle.Index = SDTAsset->GetNodeListNum(CurrentIndexHanle.NodeType) - 1;

		NewNode.NodeIndex = CurrentIndexHanle;

		// Child
		if (bExpectReturnNode && ContinueToIndex.Index != -1)
		{
			NewNode.Child = ContinueToIndex;
		}

		return;
	}

	// gather all nodes
	int32 ChildIdx = 0;
	int32 OutputIndex = -1;
	FIndexHandle CurrentIndexHanle;
	CurrentIndexHanle.Index = -1;
	CurrentIndexHanle.NodeType = ESDTNodeType::UnKnown;
	for (int32 PinIdx = 0; PinIdx < RootGraphNode->Pins.Num(); PinIdx++)
	{
		UEdGraphPin* Pin = RootGraphNode->Pins[PinIdx];
		if (Pin->Direction != EGPD_Output)
		{
			continue;
		}
		else
		{
			OutputIndex++;
		}
		// sort connections so that they're organized the same as user can see in the editor
		//Pin->LinkedTo.Sort(FCompareNodeYLocation());
		
		for (int32 LinkId = 0; LinkId < Pin->LinkedTo.Num(); ++LinkId)
		{
			
			USDTGraphNode* MyNode = Cast<USDTGraphNode>(Pin->LinkedTo[LinkId]->GetOwningNode());
			USDTGraphNode* SDTGraphNode = Cast<USDTGraphNode>(RootGraphNode);
			USDTNode* SDTNode = Cast<USDTNode>(SDTGraphNode->NodeInstance);


			if (SDTNode->NodeType == ESDTNodeType::Content)
			{
				// Conetnt
				FGameplayTag IdentityTag = Cast<USDTNode_DContent>(SDTNode)->IdentityTag;
				if (IdentityTag != FGameplayTag::EmptyTag)
				{
					SDTAsset->IdentifyTagList.Add(IdentityTag);
				}

				USDTNode_DContent* TempNode = Cast<USDTNode_DContent>(SDTGraphNode->NodeInstance);
				FSDTDContentNode& NewNode = SDTAsset->ContentNodeList.AddDefaulted_GetRef();
				CurrentIndexHanle.NodeType = Cast<USDTNode>(SDTGraphNode->NodeInstance)->NodeType;
				CurrentIndexHanle.Index = SDTAsset->GetNodeListNum(CurrentIndexHanle.NodeType) - 1;
				
				NewNode.NodeIndex = CurrentIndexHanle;
				NewNode.SourceMode = TempNode->SourceMode;
				NewNode.IdentityTag = TempNode->IdentityTag;
				NewNode.Content = TempNode->Content;
				NewNode.TimeWaitForSkip = TempNode->TimeWaitForSkip;
				// Next
				FIndexHandle NextIndexHanle;
				NextIndexHanle.NodeType = Cast<USDTNode>(MyNode->NodeInstance)->NodeType;
				NextIndexHanle.Index = SDTAsset->GetNodeListNum(NextIndexHanle.NodeType);
				NewNode.Child = NextIndexHanle;
				
				ConnectSDTNode(SDTAsset, MyNode, ContinueToIndex, ReturnToIndex, bExpectReturnNode);
			}
			else if (SDTNode->NodeType == ESDTNodeType::Selector)
			{
				// Selector
				FSDTDSelectorNode* NewNode;
				if (OutputIndex == 0)
				{
					NewNode = &(SDTAsset->SelectorNodeList.AddDefaulted_GetRef());
					CurrentIndexHanle.NodeType = Cast<USDTNode>(SDTGraphNode->NodeInstance)->NodeType;
					CurrentIndexHanle.Index = SDTAsset->GetNodeListNum(CurrentIndexHanle.NodeType) - 1;
				}
				else
				{
					NewNode = &(SDTAsset->SelectorNodeList[CurrentIndexHanle.Index]);
				}
				// Is Instanced ?
				if (SDTGraphNode->bInstancedAction)
				{
					USDTNode_DSelector_Instance* TempNode = Cast<USDTNode_DSelector_Instance>(SDTGraphNode->NodeInstance);
					NewNode->SelectorInstance = TempNode->ActionInstance;
					NewNode->SelectorClass = nullptr;
				}
				else
				{
					USDTNode_DSelector_UnInstance* TempNode = Cast<USDTNode_DSelector_UnInstance>(SDTGraphNode->NodeInstance);
					NewNode->SelectorInstance = nullptr;
					NewNode->SelectorClass = TempNode->ActionClass;
				}
				NewNode->NodeIndex = CurrentIndexHanle;
				// Next
				FIndexHandle NextIndexHanle;
				NextIndexHanle.NodeType = Cast<USDTNode>(MyNode->NodeInstance)->NodeType;
				NextIndexHanle.Index = SDTAsset->GetNodeListNum(NextIndexHanle.NodeType);
				if (OutputIndex == 0)
				{
					NewNode->TrueIndex = NextIndexHanle;
					ConnectSDTNode(SDTAsset, MyNode, ContinueToIndex, ReturnToIndex, bExpectReturnNode);
				}
				else
				{
					NewNode->FalseIndex = NextIndexHanle;
					ConnectSDTNode(SDTAsset, MyNode, ContinueToIndex, ReturnToIndex, bExpectReturnNode);
				}
				
			}
			else if (SDTNode->NodeType == ESDTNodeType::Options)
			{
				//  Options
				USDTNode_DOptions* TempNode = Cast<USDTNode_DOptions>(SDTGraphNode->NodeInstance);
				FSDTDOptionsNode* NewNode;
				if (OutputIndex == 0)
				{
					NewNode = &(SDTAsset->OptionsNodeList.AddDefaulted_GetRef());
					CurrentIndexHanle.NodeType = Cast<USDTNode>(SDTGraphNode->NodeInstance)->NodeType;
					CurrentIndexHanle.Index = SDTAsset->GetNodeListNum(CurrentIndexHanle.NodeType) - 1;
				}
				else
				{
					NewNode = &(SDTAsset->OptionsNodeList[CurrentIndexHanle.Index]);
				}
				NewNode->NodeIndex = CurrentIndexHanle;
				// Next
				FSDTDOptionItem OptionItem;
				OptionItem.Content = TempNode->Options[OutputIndex].Content;
				OptionItem.ExtraFloat.Append(TempNode->Options[OutputIndex].ExtraFloat);
				OptionItem.bSelected = false;
				OptionItem.RuleAction = TempNode->Options[OutputIndex].RuleAction;
				FIndexHandle NextIndexHanle;
				NextIndexHanle.NodeType = Cast<USDTNode>(MyNode->NodeInstance)->NodeType;
				NextIndexHanle.Index = SDTAsset->GetNodeListNum(NextIndexHanle.NodeType);
				
				NewNode->Options.Add(NextIndexHanle, OptionItem);
				NewNode->Children.Add(NextIndexHanle);

				if (bExpectReturnNode/*ReturnToIndex.Index != -1*/)
				{
					ConnectSDTNode(SDTAsset, MyNode, CurrentIndexHanle, ReturnToIndex, true);
				}
				else
				{
					ConnectSDTNode(SDTAsset, MyNode, CurrentIndexHanle, CurrentIndexHanle, true);
				}
				
			}
			else if (SDTNode->NodeType == ESDTNodeType::Action)
			{
				// Action
				FSDTDActionNode& NewNode = SDTAsset->ActionNodeList.AddDefaulted_GetRef();

				// Is Instanced ?
				if (SDTGraphNode->bInstancedAction)
				{
					USDTNode_DAction_Instance* TempNode = Cast<USDTNode_DAction_Instance>(SDTGraphNode->NodeInstance);
					NewNode.CommonActionInstance = TempNode->ActionInstance;
					NewNode.CommonActionClass = nullptr;
				}
				else
				{
					USDTNode_DAction_UnInstance* TempNode = Cast<USDTNode_DAction_UnInstance>(SDTGraphNode->NodeInstance);
					NewNode.CommonActionInstance = nullptr;
					NewNode.CommonActionClass = TempNode->ActionClass;
				}

				CurrentIndexHanle.NodeType = Cast<USDTNode>(SDTGraphNode->NodeInstance)->NodeType;
				CurrentIndexHanle.Index = SDTAsset->GetNodeListNum(CurrentIndexHanle.NodeType) - 1;
				
				NewNode.NodeIndex = CurrentIndexHanle;

				// Next
				FIndexHandle NextIndexHanle;
				NextIndexHanle.NodeType = Cast<USDTNode>(MyNode->NodeInstance)->NodeType;
				NextIndexHanle.Index = SDTAsset->GetNodeListNum(NextIndexHanle.NodeType);
				NewNode.Child = NextIndexHanle;
				
				ConnectSDTNode(SDTAsset, MyNode, ContinueToIndex, ReturnToIndex, bExpectReturnNode);
			}

		}
	}
}


#undef LOCTEXT_NAMESPACE
