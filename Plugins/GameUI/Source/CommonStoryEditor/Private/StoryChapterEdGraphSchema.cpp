// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryChapterEdGraphSchema.h"

#include "Node/SCTEditorTypes.h"
#include "Node/SCTGraphNode_Chapter.h"
#include "Node/SCTGraphNode_Entry.h"
#include "Node/SCTGraphNode_Transition.h"
#include "Graph/SCTGraph.h"
#include "SchemaAction/SCTGraphEdSchemaActions.h"
#include "Node/SCTNode_Chapter.h"
#include "Node/SCTNode_Transition.h"

#include "Framework/Commands/GenericCommands.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "StoryChapterEdGraphSchema"

int32 UStoryChapterEdGraphSchema::CurrentCacheRefreshID = 0;

UStoryChapterEdGraphSchema::UStoryChapterEdGraphSchema(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UStoryChapterEdGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	// Create the entry/exit tunnels
	FGraphNodeCreator<USCTGraphNode_Entry> NodeCreator(Graph);
	USCTGraphNode_Entry* EntryNode = NodeCreator.CreateNode();
	NodeCreator.Finalize();
	SetNodeMetaData(EntryNode, FNodeMetadata::DefaultGraphNode);
}

void UStoryChapterEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	const FName PinCategory = ContextMenuBuilder.FromPin ?
		ContextMenuBuilder.FromPin->PinType.PinCategory :
		USCTEditorTypes::PinCategory_Defualt;

	const bool bNoParent = (ContextMenuBuilder.FromPin == NULL);
	const bool bOnlyChapters = (PinCategory == USCTEditorTypes::PinCategory_Chapter);
	const bool bOnlyEntry = (PinCategory == USCTEditorTypes::PinCategory_Entry);
	const bool bAllowChapter = bNoParent || bOnlyEntry || bOnlyChapters;

	if (bAllowChapter)
	{
		TSharedPtr<FSCTSchemaAction_NewNode> Action = TSharedPtr<FSCTSchemaAction_NewNode>(
			new FSCTSchemaAction_NewNode(LOCTEXT("NewNode", "Common"), LOCTEXT("NewNode", "Chapter"), FText::GetEmpty(), 0)
			);

		USCTGraphNode_Chapter* GraphNode = NewObject<USCTGraphNode_Chapter>(ContextMenuBuilder.OwnerOfTemporaries, USCTGraphNode_Chapter::StaticClass());
		GraphNode->NodeClass = USCTNode_Chapter::StaticClass();
		Action->NodeTemplate = GraphNode;


		ContextMenuBuilder.AddAction(Action);
	}

	if (bNoParent)
	{
		TSharedPtr<FSCTSchemaAction_AutoArrange> Action = TSharedPtr<FSCTSchemaAction_AutoArrange>(
			new FSCTSchemaAction_AutoArrange(FText::GetEmpty(), LOCTEXT("AutoArrange", "Auto Arrange"), FText::GetEmpty(), 0)
			);

		ContextMenuBuilder.AddAction(Action);
	}
}

void UStoryChapterEdGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{

	if (Context->Node)
	{
		{
			FToolMenuSection& Section = Menu->AddSection("StoryChapterTreeGraphSchemaNodeActions", LOCTEXT("ClassActionsMenuHeader", "Node Actions"));
			Section.AddMenuEntry(FGenericCommands::Get().Delete);
			Section.AddMenuEntry(FGenericCommands::Get().Cut);
			Section.AddMenuEntry(FGenericCommands::Get().Copy);
			Section.AddMenuEntry(FGenericCommands::Get().Duplicate);
			Section.AddMenuEntry(FGenericCommands::Get().SelectAll);
		}
	}


	Super::GetContextMenuActions(Menu, Context);
}

FLinearColor UStoryChapterEdGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	/*const UGraphEditorSettings* Settings = GetDefault<UGraphEditorSettings>();*/
	if (PinType.PinCategory == USCTEditorTypes::PinCategory_Entry)
	{
		return FLinearColor::White;
	}
	else if (PinType.PinCategory == USCTEditorTypes::PinCategory_Chapter)
	{
		return FLinearColor::White;
	}
	else if (PinType.PinCategory == USCTEditorTypes::PinCategory_Transition)
	{
		return FLinearColor::White;
	}

	return FLinearColor::Black;
}

const FPinConnectionResponse UStoryChapterEdGraphSchema::CanCreateConnection(const UEdGraphPin* PinA, const UEdGraphPin* PinB) const
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

	const bool bPinAIsChapter = PinA->PinType.PinCategory == USCTEditorTypes::PinCategory_Chapter;
	const bool bPinAIsEntry = PinA->PinType.PinCategory == USCTEditorTypes::PinCategory_Entry;
	const bool bPinAIsTransition = PinA->PinType.PinCategory == USCTEditorTypes::PinCategory_Transition;

	const bool bPinBIsChapter = PinB->PinType.PinCategory == USCTEditorTypes::PinCategory_Chapter;
	const bool bPinBIsEntry = PinB->PinType.PinCategory == USCTEditorTypes::PinCategory_Entry;
	const bool bPinBIsTransition = PinB->PinType.PinCategory == USCTEditorTypes::PinCategory_Transition;

	//  Special case handling for entry chapters: Only allow creating connections starting at the entry chapter.
	if (bPinAIsEntry || bPinBIsEntry)
	{
		if (bPinAIsEntry && bPinBIsChapter)
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT(""));
		}

		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Entry must connect to a chapter"));
	}

	if (bPinAIsTransition && bPinBIsTransition)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot wire a transition to a transition"));
	}

	bool bDirectionsOK = false;

	// Compare the directions
	//if ((PinA->Direction == EGPD_Input) && (PinB->Direction == EGPD_Input))
	//{
	//	return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorInput", "Can't connect input node to input node"));
	//}
	//else if ((PinB->Direction == EGPD_Output) && (PinA->Direction == EGPD_Output))
	//{
	//	return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorOutput", "Can't connect output node to output node"));
	//}

	// Both directions A and B are 'Out'
	
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

	
	// Transitions are exclusive (both input and output), but chapters are not
	if (bPinAIsTransition)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT(""));
	}
	else if (bPinBIsTransition)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_B, TEXT(""));
	}
	else if (!bPinAIsTransition && !bPinBIsTransition)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE_WITH_CONVERSION_NODE, TEXT("Create a transition"));
	}


	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, LOCTEXT("PinConnect", "Connect nodes"));
}

bool UStoryChapterEdGraphSchema::TryCreateConnection(UEdGraphPin* PinA, UEdGraphPin* PinB) const
{
	// Modify the Pin LinkTo, which will affect FConnectionDrawingPolicy::DetermineLinkGeometry()
	if (PinB->Direction == PinA->Direction)
	{
		if (USCTGraphNode* Node = Cast<USCTGraphNode>(PinB->GetOwningNode()))
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

bool UStoryChapterEdGraphSchema::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* PinA, UEdGraphPin* PinB) const
{
	USCTGraphNode* NodeA = Cast<USCTGraphNode>(PinA->GetOwningNode());
	USCTGraphNode* NodeB = Cast<USCTGraphNode>(PinB->GetOwningNode());

	if ((NodeA != NULL) && (NodeB != NULL)
		&& (NodeA->GetInputPin() != NULL) && (NodeA->GetOutputPin() != NULL)
		&& (NodeB->GetInputPin() != NULL) && (NodeB->GetOutputPin() != NULL))
	{
		USCTGraphNode_Transition* NodeTemplate = NewObject<USCTGraphNode_Transition>(NodeA->GetGraph(), USCTGraphNode_Transition::StaticClass());
		NodeTemplate->NodeClass = USCTNode_Transition::StaticClass();
		
		USCTGraphNode_Transition* TransitionNode = FSCTSchemaAction_NewNode::SpawnNodeFromTemplate<USCTGraphNode_Transition>(NodeA->GetGraph(), NodeTemplate, FVector2D(0.0f, 0.0f), false);
		

		if (PinA->Direction == EGPD_Output)
		{
			TransitionNode->CreateConnections(NodeA, NodeB);
		}
		else
		{
			TransitionNode->CreateConnections(NodeB, NodeA);
		}


		return true;
	}

	return false;
}


bool UStoryChapterEdGraphSchema::IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const
{
	return CurrentCacheRefreshID != InVisualizationCacheID;
}

int32 UStoryChapterEdGraphSchema::GetCurrentVisualizationCacheID() const
{
	return CurrentCacheRefreshID;
}

void UStoryChapterEdGraphSchema::ForceVisualizationCacheClear() const
{
	++CurrentCacheRefreshID;
}

#undef LOCTEXT_NAMESPACE