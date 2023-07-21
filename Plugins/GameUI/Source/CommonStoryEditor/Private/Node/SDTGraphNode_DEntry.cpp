#include "Node/SDTGraphNode_DEntry.h"

#include "Node/SCTEditorTypes.h"

#define LOCTEXT_NAMESPACE "SDTGraphNode_DEntry"


void USDTGraphNode_DEntry::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, USDTEditorTypes::PinCategory_Entry, TEXT("Entry"));
}

FText USDTGraphNode_DEntry::GetTooltipText() const
{
	return LOCTEXT("SDTNodeTooltip", "Entry point for Story Dialogue Tree");
}

UEdGraphNode* USDTGraphNode_DEntry::GetOutputNode() const
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

UEdGraphPin* USDTGraphNode_DEntry::GetOutputPin() const
{
	Pins[0]->PinToolTip = LOCTEXT("SDTNodeTooltip", "Out").ToString();
	return Pins[0];
}


#undef LOCTEXT_NAMESPACE