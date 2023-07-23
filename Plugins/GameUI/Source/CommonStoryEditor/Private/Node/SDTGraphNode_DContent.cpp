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

void USDTGraphNode_DContent::SetContent(const FText& InText, ETextCommit::Type CommitInfo)
{
	if (USDTNode_DContent* node = Cast<USDTNode_DContent>(NodeInstance))
	{
		node->Content = InText;
	}
}

TMap<FName, FText> USDTGraphNode_DContent::GetDescription() const
{
	TMap<FName, FText> Result;
	if (USDTNode_DContent* node = Cast<USDTNode_DContent>(NodeInstance))
	{
		Result.Add(TEXT("SourceName"), FText::FromString(node->SourceMode == EDialogueSourceMode::NPC ? TEXT("NPC") : TEXT("Player")));
		Result.Add(TEXT("IdentityName"), FText::FromString(node->IdentityTag.ToString()));
	}
	
	return MoveTemp(Result);
}

UEdGraphPin* USDTGraphNode_DContent::GetInputPin() const
{
	return Pins[0];
}


UEdGraphPin* USDTGraphNode_DContent::GetOutputPin() const
{
	return Pins[1];
}



#undef LOCTEXT_NAMESPACE