#include "Node/SDTGraphNode.h"

#include "Kismet2/KismetEditorUtilities.h"
#include "Graph/SDTGraph.h"

#define LOCTEXT_NAMESPACE "SDTGraphNode"

void USDTGraphNode::InitializeNodeInstance()
{
	// empty in base class
}

void USDTGraphNode::PostPlacedNewNode()
{
	// NodeInstance can be already spawned by paste operation, don't override it

	if (NodeClass && (NodeInstance == nullptr))
	{
		UEdGraph* MyGraph = GetGraph();
		//UObject* GraphOwner = MyGraph ? MyGraph->GetOuter() : nullptr;
		if (MyGraph)
		{
			// 与 SCT不同的是，SDT不直接保存NodeInstance（仅存在于Editor状态下的Graph内）
			NodeInstance = NewObject<UObject>(MyGraph, NodeClass);

			// Redo / Undo
			/*NodeInstance->SetFlags(RF_Transactional);*/
			NodeInstance->ClearFlags(RF_Transient);
			InitializeNodeInstance();
		}
	}
}

bool USDTGraphNode::CanDuplicateNode() const
{
	return true;
}

bool USDTGraphNode::CanUserDeleteNode() const
{
	return true;
}

void USDTGraphNode::DestroyNode()
{
	UEdGraphNode::DestroyNode();
}

void USDTGraphNode::PostCopyNode()
{
	ResetNodeOwner();
}


void USDTGraphNode::PrepareForCopying()
{
	if (NodeInstance)
	{
		// Temporarily take ownership of the node instance, so that it is not deleted when cutting
		// Because until now, node has always been owned by GraphOwner (Asset)
		NodeInstance->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
	}
}

void USDTGraphNode::ResetNodeOwner()
{
	if (NodeInstance)
	{
		UEdGraph* MyGraph = GetGraph();

		// 与 SCT不同的是，SDT不直接保存NodeInstance（仅存在于Editor状态下的Graph内）
		NodeInstance->Rename(NULL, MyGraph, REN_DontCreateRedirectors | REN_DoNotDirty);
		NodeInstance->ClearFlags(RF_Transient);
	}
}

void USDTGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
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

void USDTGraphNode::NodeConnectionListChanged()
{
	Super::NodeConnectionListChanged();

	/*Cast<USDTGraph>(GetGraph())->UpdateAsset();*/
}


#undef LOCTEXT_NAMESPACE