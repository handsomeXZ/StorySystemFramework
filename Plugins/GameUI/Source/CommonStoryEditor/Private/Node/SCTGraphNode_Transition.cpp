#include "Node/SCTGraphNode_Transition.h"

#include "Node/SCTEditorTypes.h"

#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "SCTGraphNode_Transition"

USCTGraphNode_Transition::USCTGraphNode_Transition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}


void USCTGraphNode_Transition::AllocateDefaultPins()
{
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, USCTEditorTypes::PinCategory_Transition, TEXT("In"));
	Inputs->bHidden = true;
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, USCTEditorTypes::PinCategory_Transition, TEXT("Out"));
	Outputs->bHidden = true;
}

FText USCTGraphNode_Transition::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	UEdGraph* Graph = GetGraph();
	return FText::FromString(Graph->GetName());
}

FText USCTGraphNode_Transition::GetTooltipText() const
{
	return LOCTEXT("SCTChapterNodeTooltip", "This is a Transition");
}

FString USCTGraphNode_Transition::GetNodeName() const
{
	//return (BoundGraph != NULL) ? *(BoundGraph->GetName()) : TEXT("(null)");
	return TEXT("(null)");
}

void USCTGraphNode_Transition::DestroyNode()
{
	//UEdGraph* GraphToRemove = BoundGraph;

	//BoundGraph = NULL;
	Super::DestroyNode();

	/*if (GraphToRemove)
	{
		UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForNodeChecked(this);
		FBlueprintEditorUtils::RemoveGraph(Blueprint, GraphToRemove, EGraphRemoveFlags::Recompile);
	}*/
}



UEdGraphPin* USCTGraphNode_Transition::GetInputPin() const
{
	return Pins[0];
}


UEdGraphPin* USCTGraphNode_Transition::GetOutputPin() const
{
	return Pins[1];
}

USCTGraphNode* USCTGraphNode_Transition::GetPrevNode() const
{
	if (Pins[0]->LinkedTo.Num() > 0)
	{
		return Cast<USCTGraphNode>(Pins[0]->LinkedTo[0]->GetOwningNode());
	}
	else
	{
		return nullptr;
	}
}

USCTGraphNode* USCTGraphNode_Transition::GetNextNode() const
{
	if (Pins[1]->LinkedTo.Num() > 0)
	{
		return Cast<USCTGraphNode>(Pins[1]->LinkedTo[0]->GetOwningNode());
	}
	else
	{
		return nullptr;
	}
}

void USCTGraphNode_Transition::CreateConnections(USCTGraphNode* PrevNode, USCTGraphNode* NextNode)
{
	// Previous to this
	Pins[0]->Modify();
	Pins[0]->LinkedTo.Empty();

	PrevNode->GetOutputPin()->Modify();
	Pins[0]->MakeLinkTo(PrevNode->GetOutputPin());

	// This to next
	Pins[1]->Modify();
	Pins[1]->LinkedTo.Empty();

	NextNode->GetInputPin()->Modify();
	Pins[1]->MakeLinkTo(NextNode->GetInputPin());
}


#undef LOCTEXT_NAMESPACE