// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryDialogueEdGraphSchema.h"

#include "CommonStoryEditor.h"
#include "Node/SCTEditorTypes.h"
#include "Node/SDTGraphNode_DEntry.h"
#include "Node/SDTGraphNode_DExit.h"
#include "Node/SDTGraphNode_DReturn.h"
#include "Node/SDTGraphNode_DContinue.h"
#include "Node/SDTGraphNode_DContent.h"
#include "Node/SDTGraphNode_DOptions.h"
#include "Node/SDTGraphNode_DSelector.h"
#include "Node/SDTGraphNode_DAction.h"
#include "Node/SDTNode.h"
#include "Node/SDTNode_DContent.h"
#include "Node/SDTNode_DOptions.h"
#include "Node/SDTNode_DSelector_Instance.h"
#include "Node/SDTNode_DSelector_UnInstance.h"
#include "Node/SDTNode_DAction_Instance.h"
#include "Node/SDTNode_DAction_UnInstance.h"
#include "Node/SDTNode_DReturn.h"
#include "Node/SDTNode_DContinue.h"
#include "Node/SDTNode_DExit.h"
#include "Graph/SDTGraph.h"
#include "SchemaAction/SDTGraphEdSchemaActions.h"

#include "AIGraphTypes.h"

#include "Framework/Commands/GenericCommands.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "StoryDialogueEdGraphSchema"

int32 UStoryDialogueEdGraphSchema::CurrentCacheRefreshID = 0;

UStoryDialogueEdGraphSchema::UStoryDialogueEdGraphSchema(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UStoryDialogueEdGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	// Create the entry¡¢exit tunnels
	FGraphNodeCreator<USDTGraphNode_DEntry> EntryNodeCreator(Graph);
	USDTGraphNode_DEntry* EntryNode = EntryNodeCreator.CreateNode();
	EntryNodeCreator.Finalize();
	SetNodeMetaData(EntryNode, FNodeMetadata::DefaultGraphNode);

	FGraphNodeCreator<USDTGraphNode_DExit> ExitNodeCreator(Graph);
	USDTGraphNode_DExit* ExitNode = ExitNodeCreator.CreateNode();
	ExitNode->NodeClass = USDTNode_DExit::StaticClass();
	ExitNode->NodePosX = 200;
	ExitNodeCreator.Finalize();
	SetNodeMetaData(ExitNode, FNodeMetadata::DefaultGraphNode);
}

void UStoryDialogueEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	const FName PinCategory = ContextMenuBuilder.FromPin ?
		ContextMenuBuilder.FromPin->PinType.PinCategory :
		USDTEditorTypes::PinCategory_Defualt;

	const bool bNoParent = (ContextMenuBuilder.FromPin == NULL);
	const bool bOnlyContent = (PinCategory == USDTEditorTypes::PinCategory_Content);
	const bool bOnlyEntry = (PinCategory == USDTEditorTypes::PinCategory_Entry);
	const bool bOnlyReturn = (PinCategory == USDTEditorTypes::PinCategory_Return);
	const bool bOnlyContinue = (PinCategory == USDTEditorTypes::PinCategory_Continue);
	const bool bOnlyOptions = (PinCategory == USDTEditorTypes::PinCategory_Options);
	const bool bOnlySelector = (PinCategory == USDTEditorTypes::PinCategory_Selector);
	const bool bOnlyAction = (PinCategory == USDTEditorTypes::PinCategory_Action);
	const bool bAllowContent = bNoParent || bOnlyEntry || bOnlyContent || bOnlyOptions || bOnlySelector || bOnlyAction;
	const bool bAllowOptions = bNoParent || bOnlyEntry || bOnlyContent || bOnlyOptions || bOnlySelector || bOnlyAction;
	const bool bAllowSelector = bNoParent || bOnlyEntry || bOnlyContent || bOnlyOptions || bOnlySelector || bOnlyAction;
	const bool bAllowAction = bNoParent || bOnlyEntry || bOnlyContent || bOnlyOptions || bOnlySelector || bOnlyAction;
	const bool bAllowReturn = bNoParent || bOnlyContent || bOnlyOptions || bOnlySelector || bOnlyAction || bOnlyReturn;
	const bool bAllowContinue = bNoParent || bOnlyContent || bOnlyOptions || bOnlySelector || bOnlyAction || bOnlyContinue;

	if (bAllowContent)
	{
		TSharedPtr<FSDTSchemaAction_NewNode> Action = TSharedPtr<FSDTSchemaAction_NewNode>(
			new FSDTSchemaAction_NewNode(LOCTEXT("NewNode", "Common"), LOCTEXT("NewNode", "Content"), FText::GetEmpty(), 0)
			);

		USDTGraphNode_DContent* GraphNode = NewObject<USDTGraphNode_DContent>(ContextMenuBuilder.OwnerOfTemporaries, USDTGraphNode_DContent::StaticClass());
		GraphNode->NodeClass = USDTNode_DContent::StaticClass();
		Action->NodeTemplate = GraphNode;


		ContextMenuBuilder.AddAction(Action);
	}

	if (bAllowOptions)
	{
		TSharedPtr<FSDTSchemaAction_NewNode> Action = TSharedPtr<FSDTSchemaAction_NewNode>(
			new FSDTSchemaAction_NewNode(LOCTEXT("NewNode", "Dialogue Options"), LOCTEXT("NewNode", "Options"), FText::GetEmpty(), 0)
			);

		USDTGraphNode_DOptions* GraphNode = NewObject<USDTGraphNode_DOptions>(ContextMenuBuilder.OwnerOfTemporaries, USDTGraphNode_DOptions::StaticClass());
		GraphNode->NodeClass = USDTNode_DOptions::StaticClass();
		Action->NodeTemplate = GraphNode;


		ContextMenuBuilder.AddAction(Action);
	}

	if (bAllowReturn)
	{
		TSharedPtr<FSDTSchemaAction_NewNode> Action = TSharedPtr<FSDTSchemaAction_NewNode>(
			new FSDTSchemaAction_NewNode(LOCTEXT("NewNode", "Dialogue Options"), LOCTEXT("NewNode", "Return"), FText::GetEmpty(), 0)
			);

		USDTGraphNode_DReturn* GraphNode = NewObject<USDTGraphNode_DReturn>(ContextMenuBuilder.OwnerOfTemporaries, USDTGraphNode_DReturn::StaticClass());
		GraphNode->NodeClass = USDTNode_DReturn::StaticClass();
		Action->NodeTemplate = GraphNode;


		ContextMenuBuilder.AddAction(Action);
	}

	if (bAllowContinue)
	{
		TSharedPtr<FSDTSchemaAction_NewNode> Action = TSharedPtr<FSDTSchemaAction_NewNode>(
			new FSDTSchemaAction_NewNode(LOCTEXT("NewNode", "Dialogue Options"), LOCTEXT("NewNode", "Continue"), FText::GetEmpty(), 0)
			);

		USDTGraphNode_DContinue* GraphNode = NewObject<USDTGraphNode_DContinue>(ContextMenuBuilder.OwnerOfTemporaries, USDTGraphNode_DContinue::StaticClass());
		GraphNode->NodeClass = USDTNode_DContinue::StaticClass();
		Action->NodeTemplate = GraphNode;


		ContextMenuBuilder.AddAction(Action);
	}

	if (bAllowSelector)
	{
		FCommonStoryEditorModule& EditorModule = FModuleManager::GetModuleChecked<FCommonStoryEditorModule>(TEXT("CommonStoryEditor"));
		FGraphNodeClassHelper* ClassCache = EditorModule.GetClassCache().Get();

		TArray<FGraphNodeClassData> ActionClasses;
		ClassCache->GatherClasses(USDTDSelectorAction::StaticClass(), ActionClasses);

		for (auto& ActionClass : ActionClasses)
		{
			const FText ActionTypeName = FText::FromString(FName::NameToDisplayString(ActionClass.ToString(), false));

			TSharedPtr<FSDTSchemaAction_NewNode> Action = TSharedPtr<FSDTSchemaAction_NewNode>(
				new FSDTSchemaAction_NewNode(LOCTEXT("NewNode", "Selector_Instance"), ActionTypeName, FText::GetEmpty(), 0)
				);
			USDTGraphNode_DSelector* GraphNode = NewObject<USDTGraphNode_DSelector>(ContextMenuBuilder.OwnerOfTemporaries, USDTGraphNode_DSelector::StaticClass());
			GraphNode->bInstancedAction = true;
			GraphNode->ActionClass = ActionClass.GetClass();
			GraphNode->NodeClass = USDTNode_DSelector_Instance::StaticClass();
			Action->NodeTemplate = GraphNode;

			ContextMenuBuilder.AddAction(Action);

			Action = TSharedPtr<FSDTSchemaAction_NewNode>(
				new FSDTSchemaAction_NewNode(LOCTEXT("NewNode", "Selector_UnInstance"), ActionTypeName, FText::GetEmpty(), 0)
				);

			GraphNode = NewObject<USDTGraphNode_DSelector>(ContextMenuBuilder.OwnerOfTemporaries, USDTGraphNode_DSelector::StaticClass());
			GraphNode->bInstancedAction = false;
			GraphNode->ActionClass = ActionClass.GetClass();
			GraphNode->NodeClass = USDTNode_DSelector_UnInstance::StaticClass();
			Action->NodeTemplate = GraphNode;

			ContextMenuBuilder.AddAction(Action);
		}
	}

	if (bAllowAction)
	{
		FCommonStoryEditorModule& EditorModule = FModuleManager::GetModuleChecked<FCommonStoryEditorModule>(TEXT("CommonStoryEditor"));
		FGraphNodeClassHelper* ClassCache = EditorModule.GetClassCache().Get();

		TArray<FGraphNodeClassData> ActionClasses;
		ClassCache->GatherClasses(USDTDCommonAction::StaticClass(), ActionClasses);

		for (auto& ActionClass : ActionClasses)
		{
			const FText ActionTypeName = FText::FromString(FName::NameToDisplayString(ActionClass.ToString(), false));

			TSharedPtr<FSDTSchemaAction_NewNode> Action = TSharedPtr<FSDTSchemaAction_NewNode>(
				new FSDTSchemaAction_NewNode(LOCTEXT("NewNode", "CommonAction_Instance"), ActionTypeName, FText::GetEmpty(), 0)
				);
			USDTGraphNode_DAction* GraphNode = NewObject<USDTGraphNode_DAction>(ContextMenuBuilder.OwnerOfTemporaries, USDTGraphNode_DAction::StaticClass());
			GraphNode->bInstancedAction = true;
			GraphNode->ActionClass = ActionClass.GetClass();
			GraphNode->NodeClass = USDTNode_DAction_Instance::StaticClass();
			Action->NodeTemplate = GraphNode;

			ContextMenuBuilder.AddAction(Action);

			Action = TSharedPtr<FSDTSchemaAction_NewNode>(
				new FSDTSchemaAction_NewNode(LOCTEXT("NewNode", "CommonAction_UnInstance"), ActionTypeName, FText::GetEmpty(), 0)
				);

			GraphNode = NewObject<USDTGraphNode_DAction>(ContextMenuBuilder.OwnerOfTemporaries, USDTGraphNode_DAction::StaticClass());
			GraphNode->bInstancedAction = false;
			GraphNode->ActionClass = ActionClass.GetClass();
			GraphNode->NodeClass = USDTNode_DAction_UnInstance::StaticClass();
			Action->NodeTemplate = GraphNode;

			ContextMenuBuilder.AddAction(Action);
		}
	}
}

void UStoryDialogueEdGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{

	if (Context->Node)
	{
		{
			FToolMenuSection& Section = Menu->AddSection("StoryDialogueTreeGraphSchemaNodeActions", LOCTEXT("ClassActionsMenuHeader", "Node Actions"));
			Section.AddMenuEntry(FGenericCommands::Get().Delete);
			Section.AddMenuEntry(FGenericCommands::Get().Cut);
			Section.AddMenuEntry(FGenericCommands::Get().Copy);
			Section.AddMenuEntry(FGenericCommands::Get().Duplicate);
			Section.AddMenuEntry(FGenericCommands::Get().SelectAll);
		}
	}


	Super::GetContextMenuActions(Menu, Context);
}

FLinearColor UStoryDialogueEdGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	/*const UGraphEditorSettings* Settings = GetDefault<UGraphEditorSettings>();*/
	if (PinType.PinCategory == USDTEditorTypes::PinCategory_Entry)
	{
		return FLinearColor::White;
	}
	else if (PinType.PinCategory == USDTEditorTypes::PinCategory_Content)
	{
		return FLinearColor::White;
	}
	else if (PinType.PinCategory == USDTEditorTypes::PinCategory_Options)
	{
		return FLinearColor::White;
	}

	return FLinearColor::Black;
}

const FPinConnectionResponse UStoryDialogueEdGraphSchema::CanCreateConnection(const UEdGraphPin* PinA, const UEdGraphPin* PinB) const
{
	if (!PinA || !PinB)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT(""));
	}

	// Make sure the pins are not on the same node
	if (PinA->GetOwningNode() == PinB->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorSameNode", "Both are on the same node"));
	}

	// Compare the directions
	if (PinA->Direction == EGPD_Input)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorInput", "Can't connect node start from input node"));
	}
	else if ((PinA->Direction == EGPD_Input) && (PinB->Direction == EGPD_Input))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorInput", "Can't connect input node to input node"));
	}
	else if ((PinB->Direction == EGPD_Output) && (PinA->Direction == EGPD_Output))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorOutput", "Can't connect output node to output node"));
	}

	const bool bPinAIsContent = PinA->PinType.PinCategory == USDTEditorTypes::PinCategory_Content;
	const bool bPinAIsEntry = PinA->PinType.PinCategory == USDTEditorTypes::PinCategory_Entry;
	const bool bPinAIsOptions = PinA->PinType.PinCategory == USDTEditorTypes::PinCategory_Options;
	const bool bPinAIsSelector = PinA->PinType.PinCategory == USDTEditorTypes::PinCategory_Selector;
	const bool bPinAIsAction = PinA->PinType.PinCategory == USDTEditorTypes::PinCategory_Action;
	const bool bPinAIsExit = PinA->PinType.PinCategory == USDTEditorTypes::PinCategory_Exit;
	const bool bPinAIsReturn = PinA->PinType.PinCategory == USDTEditorTypes::PinCategory_Return;
	const bool bPinAIsContinue = PinA->PinType.PinCategory == USDTEditorTypes::PinCategory_Continue;

	const bool bPinBIsContent = PinB->PinType.PinCategory == USDTEditorTypes::PinCategory_Content;
	const bool bPinBIsEntry = PinB->PinType.PinCategory == USDTEditorTypes::PinCategory_Entry;
	const bool bPinBIsOptions = PinB->PinType.PinCategory == USDTEditorTypes::PinCategory_Options;
	const bool bPinBIsSelector = PinB->PinType.PinCategory == USDTEditorTypes::PinCategory_Selector;
	const bool bPinBIsAction = PinB->PinType.PinCategory == USDTEditorTypes::PinCategory_Action;
	const bool bPinBIsExit = PinB->PinType.PinCategory == USDTEditorTypes::PinCategory_Exit;
	const bool bPinBIsReturn = PinB->PinType.PinCategory == USDTEditorTypes::PinCategory_Return;
	const bool bPinBIsContinue = PinB->PinType.PinCategory == USDTEditorTypes::PinCategory_Continue;

	if (bPinAIsEntry || bPinBIsEntry)
	{
		if (bPinAIsEntry && (bPinBIsContent || bPinBIsOptions || bPinBIsSelector || bPinBIsAction))
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT(""));
		}

		if (bPinBIsExit)
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT(""));
		}

		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Entry Node can't connect to this node"));
	}

	if (bPinAIsReturn || bPinBIsReturn)
	{
		if (bPinBIsReturn && (bPinAIsContent || bPinAIsOptions || bPinAIsSelector || bPinAIsAction))
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
		}

		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Return Node can't connect to this node"));
	}

	if (bPinAIsContinue || bPinBIsContinue)
	{
		if (bPinBIsContinue && (bPinAIsContent || bPinAIsOptions || bPinAIsSelector || bPinAIsAction))
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
		}

		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Continue Node can't connect to this node"));
	}

	class FNodeVisitorCycleChecker
	{
	public:
		/** Check whether a loop in the graph would be caused by linking the passed-in nodes */
		bool CheckForLoop(UEdGraphNode* StartNode, UEdGraphNode* EndNode)
		{
			VisitedNodes.Add(EndNode);
			return TraverseInputNodesToRoot(StartNode);
		}

	private:
		/**
		 * Helper function for CheckForLoop()
		 * @param	Node	The node to start traversal at
		 * @return true if we reached a root node (i.e. a node with no input pins), false if we encounter a node we have already seen
		 */
		bool TraverseInputNodesToRoot(UEdGraphNode* Node)
		{
			VisitedNodes.Add(Node);

			// Follow every input pin until we cant any more ('root') or we reach a node we have seen (cycle)
			for (int32 PinIndex = 0; PinIndex < Node->Pins.Num(); ++PinIndex)
			{
				UEdGraphPin* MyPin = Node->Pins[PinIndex];

				if (MyPin->Direction == EGPD_Input)
				{
					for (int32 LinkedPinIndex = 0; LinkedPinIndex < MyPin->LinkedTo.Num(); ++LinkedPinIndex)
					{
						UEdGraphPin* OtherPin = MyPin->LinkedTo[LinkedPinIndex];
						if (OtherPin)
						{
							UEdGraphNode* OtherNode = OtherPin->GetOwningNode();
							if (VisitedNodes.Contains(OtherNode))
							{
								return false;
							}
							else
							{
								return TraverseInputNodesToRoot(OtherNode);
							}
						}
					}
				}
			}

			return true;
		}

		TSet<UEdGraphNode*> VisitedNodes;
	};

	// check for cycles
	FNodeVisitorCycleChecker CycleChecker;
	if (!CycleChecker.CheckForLoop(PinA->GetOwningNode(), PinB->GetOwningNode()))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorcycle", "Can't create a graph cycle"));
	}
	
	if (PinA->Direction == EGPD_Output && PinA->LinkedTo.Num() > 0)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, LOCTEXT("PinConnectReplace", "Replace connection"));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, LOCTEXT("PinConnect", "Connect nodes"));
}

bool UStoryDialogueEdGraphSchema::TryCreateConnection(UEdGraphPin* PinA, UEdGraphPin* PinB) const
{
	// Modify the Pin LinkTo, which will affect FConnectionDrawingPolicy::DetermineLinkGeometry()
	if (PinB->Direction == PinA->Direction)
	{
		if (USDTGraphNode* Node = Cast<USDTGraphNode>(PinB->GetOwningNode()))
		{
			if (PinA->Direction == EGPD_Input)
			{
				PinB = Node->GetOutputPin();
			}
			else
			{
				PinB = Node->GetInputPin();
			}
		}
	}

	const bool bModified = UEdGraphSchema::TryCreateConnection(PinA, PinB);

	return bModified;
}

TSharedPtr<FEdGraphSchemaAction> UStoryDialogueEdGraphSchema::GetCreateCommentAction() const
{
	return TSharedPtr<FEdGraphSchemaAction>(static_cast<FEdGraphSchemaAction*>(new FSDTSchemaAction_AddComment));
}

bool UStoryDialogueEdGraphSchema::IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const
{
	return CurrentCacheRefreshID != InVisualizationCacheID;
}

int32 UStoryDialogueEdGraphSchema::GetCurrentVisualizationCacheID() const
{
	return CurrentCacheRefreshID;
}

void UStoryDialogueEdGraphSchema::ForceVisualizationCacheClear() const
{
	++CurrentCacheRefreshID;
}

#undef LOCTEXT_NAMESPACE