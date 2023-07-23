#include "Node/SDTGraphNode_DAction.h"

#include "Node/SCTEditorTypes.h"
#include "Node/SDTNode_DAction_Instance.h"
#include "Node/SDTNode_DAction_UnInstance.h"

#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "SCTGraphNode_Entry"



void USDTGraphNode_DAction::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, USDTEditorTypes::PinCategory_Action, TEXT("In"));
	CreatePin(EGPD_Output, USDTEditorTypes::PinCategory_Action, TEXT("Out"));
}

FText USDTGraphNode_DAction::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (bInstancedAction)
	{
		USDTNode_DAction_Instance* MyNode = Cast<USDTNode_DAction_Instance>(NodeInstance);
		if (MyNode->NodeName.Len() > 0)
		{
			return FText::FromString(MyNode->NodeName);
		}
	}
	else
	{
		USDTNode_DAction_UnInstance* MyNode = Cast<USDTNode_DAction_UnInstance>(NodeInstance);
		if (MyNode->NodeName.Len() > 0)
		{
			return FText::FromString(MyNode->NodeName);
		}
	}

	return FText::FromString(FName::NameToDisplayString(SCTClassUtils::ClassToString(GetActionDataClass()), false));
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

UClass* USDTGraphNode_DAction::GetActionDataClass() const
{
	if (bInstancedAction)
	{
		USDTNode_DAction_Instance* Action = Cast<USDTNode_DAction_Instance>(NodeInstance);
		if (Action->ActionInstance)
		{
			return Action->ActionInstance->GetClass();
		}
	}
	else
	{
		USDTNode_DAction_UnInstance* Action = Cast<USDTNode_DAction_UnInstance>(NodeInstance);
		if (Action->ActionClass)
		{
			return Action->ActionClass;
		}
	}
	return ActionClass;
}

void USDTGraphNode_DAction::InitializeNodeInstance()
{
	if (bInstancedAction)
	{
		USDTNode_DAction_Instance* Action = Cast<USDTNode_DAction_Instance>(NodeInstance);
		Action->ActionInstance = NewObject<USDTDCommonAction>(NodeInstance, ActionClass);
	}
	else
	{
		USDTNode_DAction_UnInstance* Action = Cast<USDTNode_DAction_UnInstance>(NodeInstance);
		Action->ActionClass = ActionClass;
	}
}


#undef LOCTEXT_NAMESPACE