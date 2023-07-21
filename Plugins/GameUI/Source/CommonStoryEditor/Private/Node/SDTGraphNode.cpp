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

void USDTGraphNode::PrepareForCopying()
{
	
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

void USDTGraphNode::ResetNodeOwner()
{
	if (NodeInstance)
	{
		UEdGraph* MyGraph = GetGraph();
		UObject* GraphOwner = MyGraph ? MyGraph->GetOuter() : nullptr;

		NodeInstance->Rename(NULL, GraphOwner, REN_DontCreateRedirectors | REN_DoNotDirty);
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