#include "Node/SDTGraphNode_DSelector.h"

#include "Node/SCTEditorTypes.h"
#include "Node/SDTNode_DSelector_Instance.h"
#include "Node/SDTNode_DSelector_UnInstance.h"
#include "Asset/StoryDialogueTree.h"

#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "SDTGraphNode_DSelector"



void USDTGraphNode_DSelector::PostPlacedNewNode()
{
	// NodeInstance can be already spawned by paste operation, don't override it

	if (NodeClass && (NodeInstance == nullptr))
	{
		UEdGraph* MyGraph = GetGraph();
		UObject* GraphOwner = MyGraph ? MyGraph->GetOuter() : nullptr;
		if (GraphOwner)
		{
			NodeInstance = NewObject<UObject>(GraphOwner, NodeClass);
			// Redo / Undo
			/*NodeInstance->SetFlags(RF_Transactional);*/

			InitializeNodeInstance();
		}
	}
}

void USDTGraphNode_DSelector::InitializeNodeInstance()
{
	if (bInstancedAction)
	{
		USDTNode_DSelector_Instance* MyNodeInstance = Cast<USDTNode_DSelector_Instance>(NodeInstance);
		UEdGraph* MyGraph = GetGraph();
		UObject* GraphOwner = MyGraph ? MyGraph->GetOuter() : nullptr;
		if (GraphOwner)
		{
			MyNodeInstance->ActionInstance = NewObject<USDTDSelectorAction>(GraphOwner, ActionClass);
		}
	}
	else
	{
		USDTNode_DSelector_UnInstance* MyNodeInstance = Cast<USDTNode_DSelector_UnInstance>(NodeInstance);
		UEdGraph* MyGraph = GetGraph();
		UObject* GraphOwner = MyGraph ? MyGraph->GetOuter() : nullptr;
		if (GraphOwner)
		{
			MyNodeInstance->ActionClass = ActionClass;
		}
	}
}

void USDTGraphNode_DSelector::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, USDTEditorTypes::PinCategory_Selector, TEXT("In"));
	CreatePin(EGPD_Output, USDTEditorTypes::PinCategory_Selector, TEXT("True"));
	CreatePin(EGPD_Output, USDTEditorTypes::PinCategory_Selector, TEXT("False"));
}

FText USDTGraphNode_DSelector::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(FName::NameToDisplayString(SCTClassUtils::ClassToString(ActionClass), false));
}

FText USDTGraphNode_DSelector::GetTooltipText() const
{
	return LOCTEXT("SDTNodeTooltip", "This is a Dialogue Selector");
}

FString USDTGraphNode_DSelector::GetNodeName() const
{
	return TEXT("Dialogue Selector");
}

UEdGraphPin* USDTGraphNode_DSelector::GetInputPin() const
{
	return Pins[0];
}


UEdGraphPin* USDTGraphNode_DSelector::GetTrueOutputPin() const
{
	return Pins[1];
}

UEdGraphPin* USDTGraphNode_DSelector::GetFalseOutputPin() const
{
	return Pins[2];
}

#undef LOCTEXT_NAMESPACE