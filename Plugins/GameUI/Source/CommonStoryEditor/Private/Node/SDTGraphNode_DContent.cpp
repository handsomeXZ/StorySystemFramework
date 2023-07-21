#include "Node/SDTGraphNode_DContent.h"

#include "Node/SCTEditorTypes.h"
#include "Node/SDTNode_DContent.h"

#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "SCTGraphNode_Entry"

void USDTGraphNode_DContent::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, USDTEditorTypes::PinCategory_Content, TEXT("In"));
	CreatePin(EGPD_Output, USDTEditorTypes::PinCategory_Content, TEXT("Out"));
}

FText USDTGraphNode_DContent::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Dialogue Content"));
}

FText USDTGraphNode_DContent::GetTooltipText() const
{
	return LOCTEXT("SDTNodeTooltip", "This is a Dialogue Content");
}

FString USDTGraphNode_DContent::GetNodeName() const
{
	return TEXT("Dialogue Content");
}

FText USDTGraphNode_DContent::GetContent() const
{
	if (USDTNode_DContent* node = Cast<USDTNode_DContent>(NodeInstance))
	{
		return node->Content;
	}
	return FText::FromString(TEXT("NULL"));
}

UEdGraphPin* USDTGraphNode_DContent::GetInputPin() const
{
	Pins[0]->PinToolTip = LOCTEXT("SDTNodeTooltip", "In").ToString();
	return Pins[0];
}


UEdGraphPin* USDTGraphNode_DContent::GetOutputPin() const
{
	Pins[1]->PinToolTip = LOCTEXT("SDTNodeTooltip", "Out").ToString();
	return Pins[1];
}



#undef LOCTEXT_NAMESPACE