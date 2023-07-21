#include "Node/SDTGraphNode_DContinue.h"

#include "Node/SCTEditorTypes.h"

#define LOCTEXT_NAMESPACE "SDTGraphNode_DContinue"


void USDTGraphNode_DContinue::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, USDTEditorTypes::PinCategory_Return, TEXT("Continue"));
}

FText USDTGraphNode_DContinue::GetTooltipText() const
{
	return LOCTEXT("SDTNodeTooltip", "Continue point for Story Dialogue Tree");
}

UEdGraphNode* USDTGraphNode_DContinue::GetOutputNode() const
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

UEdGraphPin* USDTGraphNode_DContinue::GetInputPin() const
{
	Pins[0]->PinToolTip = LOCTEXT("SDTNodeTooltip", "In").ToString();
	return Pins[0];
}


#undef LOCTEXT_NAMESPACE