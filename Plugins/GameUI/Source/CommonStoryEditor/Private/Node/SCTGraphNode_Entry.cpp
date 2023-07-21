#include "Node/SCTGraphNode_Entry.h"

#include "Node/SCTEditorTypes.h"

#define LOCTEXT_NAMESPACE "SCTGraphNode_Entry"

USCTGraphNode_Entry::USCTGraphNode_Entry(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void USCTGraphNode_Entry::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, USCTEditorTypes::PinCategory_Entry, TEXT("Entry"));
}

FText USCTGraphNode_Entry::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	UEdGraph* Graph = GetGraph();
	return FText::FromString(Graph->GetName());
}

FText USCTGraphNode_Entry::GetTooltipText() const
{
	return LOCTEXT("SCTEntryNodeTooltip", "Entry point for Story Chapter Tree");
}

UEdGraphNode* USCTGraphNode_Entry::GetOutputNode() const
{
	if (Pins.Num() > 0 && Pins[0] != NULL)
	{
		check(Pins[0]->LinkedTo.Num() <= 1);
		if (Pins[0]->LinkedTo.Num() > 0 && Pins[0]->LinkedTo[0]->GetOwningNode() != NULL)
		{
			return Pins[0]->LinkedTo[0]->GetOwningNode();
		}
	}
	return NULL;
}

UEdGraphPin* USCTGraphNode_Entry::GetOutputPin() const
{
	return Pins[0];
}

#undef LOCTEXT_NAMESPACE