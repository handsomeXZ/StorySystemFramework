#include "Node/SDTGraphNode_DOptions.h"

#include "Node/SCTEditorTypes.h"
#include "Node/SDTNode_DOptions.h"

#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "SCTGraphNode_Entry"


void USDTGraphNode_DOptions::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, USDTEditorTypes::PinCategory_Options, TEXT("In"));
}

FText USDTGraphNode_DOptions::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Options"));
}

FText USDTGraphNode_DOptions::GetTooltipText() const
{
	return LOCTEXT("SDTNodeTooltip", "This is a Dialogue Options");
}

FString USDTGraphNode_DOptions::GetNodeName() const
{
	return TEXT("Dialogue Options");
}

UEdGraphPin* USDTGraphNode_DOptions::GetInputPin() const
{
	return Pins[0];
}


TArray<UEdGraphPin*> USDTGraphNode_DOptions::GetOutputPins() const
{
	TArray<UEdGraphPin*> OutPins;
	
	OutPins.Append(Pins);
	OutPins.RemoveAt(0);

	return OutPins;
}

void USDTGraphNode_DOptions::AddPinToNode()
{
	USDTNode_DOptions* Node = Cast<USDTNode_DOptions>(NodeInstance);
	FSDTNode_DOptionItem Item;
	Item.Content = FText::FromString(TEXT("empty"));
	Node->Options.Add(Item);
	CreatePin(EGPD_Output, USDTEditorTypes::PinCategory_Options, TEXT("Out"));
}

TArray<FSDTNode_DOptionItem>& USDTGraphNode_DOptions::GetOutputPinsName() const
{
	USDTNode_DOptions* Node = Cast<USDTNode_DOptions>(NodeInstance);

	return Node->Options;
}

#undef LOCTEXT_NAMESPACE