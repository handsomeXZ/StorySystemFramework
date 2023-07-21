#include "Node/SDTGraphNode_DAction.h"

#include "Node/SCTEditorTypes.h"

#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "SCTGraphNode_Entry"



void USDTGraphNode_DAction::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, USDTEditorTypes::PinCategory_Action, TEXT("In"));
	CreatePin(EGPD_Output, USDTEditorTypes::PinCategory_Action, TEXT("Out"));
}

FText USDTGraphNode_DAction::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(FName::NameToDisplayString(SCTClassUtils::ClassToString(ActionClass), false));
}

FText USDTGraphNode_DAction::GetTooltipText() const
{
	return LOCTEXT("SDTNodeTooltip", "This is a Dialogue Action");
}

FString USDTGraphNode_DAction::GetNodeName() const
{
	return TEXT("Dialogue Action");
}

UEdGraphPin* USDTGraphNode_DAction::GetInputPin() const
{
	return Pins[0];
}


UEdGraphPin* USDTGraphNode_DAction::GetOutputPin() const
{
	return Pins[1];
}



#undef LOCTEXT_NAMESPACE