// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/SCTGraphNode.h"

#include "Kismet2/KismetEditorUtilities.h"
#include "Graph/SCTGraph.h"

#include "Node/SCTGraphNode_Transition.h"

#define LOCTEXT_NAMESPACE "SCTGraphNode"

void USCTGraphNode::InitializeNodeInstance()
{
	// empty in base class
}

void USCTGraphNode::PostPasteNode()
{
	Super::PostPasteNode();

	for (UEdGraph* SubGraph : GetSubGraphs())
	{
		if (SubGraph)
		{
			// Add the new graph as a child of our parent graph
			UEdGraph* ParentGraph = GetGraph();

			if (ParentGraph->SubGraphs.Find(SubGraph) == INDEX_NONE)
			{
				ParentGraph->SubGraphs.Add(SubGraph);
			}
		}
	}
}

void USCTGraphNode::PostPlacedNewNode()
{
	// NodeInstance can be already spawned by paste operation, don't override it

	if (NodeClass && (NodeInstance == nullptr))
	{
		UEdGraph* MyGraph = GetGraph();
		UObject* GraphOwner = MyGraph ? MyGraph->GetOuter() : nullptr;
		if (GraphOwner)
		{
			NodeInstance = NewObject<UObject>(GraphOwner, NodeClass);

			// Redo / Undo
			/*NodeInstance->SetFlags(RF_Transactional);*/
			NodeInstance->ClearFlags(RF_Transient);
			InitializeNodeInstance();
		}
	}
}

UObject* USCTGraphNode::GetJumpTargetForDoubleClick() const
{
	TArray<UEdGraph*> SubGraphs = GetSubGraphs();
	check(SubGraphs.Num() > 0);
	return SubGraphs[0];
}

bool USCTGraphNode::CanJumpToDefinition() const
{
	return GetJumpTargetForDoubleClick() != nullptr;
}

void USCTGraphNode::JumpToDefinition() const
{
	if (UObject* HyperlinkTarget = GetJumpTargetForDoubleClick())
	{
		FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(HyperlinkTarget);
	}
}

void USCTGraphNode::PrepareForCopying()
{
	//if (NodeInstance)
	//{
	//	// Temporarily take ownership of the node instance, so that it is not deleted when cutting
	//	NodeInstance->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
	//}
}

bool USCTGraphNode::CanDuplicateNode() const
{
	return Super::CanDuplicateNode();
}

bool USCTGraphNode::CanUserDeleteNode() const
{
	return Super::CanUserDeleteNode();
}

void USCTGraphNode::DestroyNode()
{
	UEdGraphNode::DestroyNode();
}

void USCTGraphNode::PostCopyNode()
{
	ResetNodeOwner();
}

void USCTGraphNode::ResetNodeOwner()
{
	if (NodeInstance)
	{
		UEdGraph* MyGraph = GetGraph();
		UObject* GraphOwner = MyGraph ? MyGraph->GetOuter() : nullptr;

		NodeInstance->Rename(NULL, GraphOwner, REN_DontCreateRedirectors | REN_DoNotDirty);
		NodeInstance->ClearFlags(RF_Transient);
	}
}

void USCTGraphNode::GetTransitionList(TArray<USCTGraphNode_Transition*>& OutTransitions) const
{
	// Normal transitions
	for (int32 LinkIndex = 0; LinkIndex < Pins[1]->LinkedTo.Num(); ++LinkIndex)
	{
		UEdGraphNode* TargetNode = Pins[1]->LinkedTo[LinkIndex]->GetOwningNode();
		if (USCTGraphNode_Transition* Transition = Cast<USCTGraphNode_Transition>(TargetNode))
		{
			OutTransitions.Add(Transition);
		}
	}
}

void USCTGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);

	if (FromPin != nullptr)
	{
		UEdGraphPin* OutputPin = GetOutputPin();

		if (GetSchema()->TryCreateConnection(FromPin, GetInputPin()))
		{
			FromPin->GetOwningNode()->NodeConnectionListChanged();
		}
		else if (OutputPin != nullptr && GetSchema()->TryCreateConnection(OutputPin, FromPin))
		{
			NodeConnectionListChanged();
		}
	}
}

void USCTGraphNode::NodeConnectionListChanged()
{
	Super::NodeConnectionListChanged();

	/*Cast<USCTGraph>(GetGraph())->UpdateAsset();*/
}


#undef LOCTEXT_NAMESPACE