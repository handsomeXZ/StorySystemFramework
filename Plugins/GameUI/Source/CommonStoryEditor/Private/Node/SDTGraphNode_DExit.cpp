#include "Node/SDTGraphNode_DExit.h"

#include "Node/SCTEditorTypes.h"

#define LOCTEXT_NAMESPACE "SDTGraphNode_DEntry"


void USDTGraphNode_DExit::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, USDTEditorTypes::PinCategory_Exit, TEXT("Exit"));
}

FText USDTGraphNode_DExit::GetTooltipText() const
{
	return LOCTEXT("SDTNodeTooltip", "Exit point for Story Dialogue Tree");
}

UEdGraphNode* USDTGraphNode_DExit::GetOutputNode() const
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

UEdGraphPin* USDTGraphNode_DExit::GetInputPin() const
{
	Pins[0]->PinToolTip = LOCTEXT("SDTNodeTooltip", "In").ToString();
	return Pins[0];
}


#undef LOCTEXT_NAMESPACE