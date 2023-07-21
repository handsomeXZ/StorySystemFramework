#include "Node/SCTGraphNode_Chapter.h"

#include "Node/SCTEditorTypes.h"
#include "Node/SCTNode_Chapter.h"

#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "SCTGraphNode_Entry"

USCTGraphNode_Chapter::USCTGraphNode_Chapter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void USCTGraphNode_Chapter::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, USCTEditorTypes::PinCategory_Chapter, TEXT("In"));
	CreatePin(EGPD_Output, USCTEditorTypes::PinCategory_Chapter, TEXT("Out"));
}

FText USCTGraphNode_Chapter::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(
		NodeInstance != nullptr ?
			Cast<USCTNode_Chapter>(NodeInstance)->ChapterName : TEXT("Chapter")
		);
}

FText USCTGraphNode_Chapter::GetTooltipText() const
{
	return LOCTEXT("SCTChapterNodeTooltip", "This is a Chapter");
}

FString USCTGraphNode_Chapter::GetNodeName() const
{
	return (BoundGraph != NULL) ? *(BoundGraph->GetName()) : TEXT("(null)");
}

void USCTGraphNode_Chapter::PostPasteNode()
{
	// Find an interesting name, but try to keep the same if possible
	//TSharedPtr<INameValidatorInterface> NameValidator = FNameValidatorFactory::MakeValidator(this);
	//FBlueprintEditorUtils::RenameGraphWithSuggestion(BoundGraph, NameValidator, GetStateName());

	//for (UEdGraphNode* GraphNode : BoundGraph->Nodes)
	//{
	//	GraphNode->CreateNewGuid();
	//	GraphNode->PostPasteNode();
	//	GraphNode->ReconstructNode();
	//}

	Super::PostPasteNode();
}

void USCTGraphNode_Chapter::PostPlacedNewNode()
{
	Super::PostPlacedNewNode();
	// Create a new animation graph
	//check(BoundGraph == NULL);
	//BoundGraph = FBlueprintEditorUtils::CreateNewGraph(
	//	this,
	//	NAME_None,
	//	UAnimationStateGraph::StaticClass(),
	//	UAnimationStateGraphSchema::StaticClass());
	//check(BoundGraph);

	//// Find an interesting name
	//TSharedPtr<INameValidatorInterface> NameValidator = FNameValidatorFactory::MakeValidator(this);
	//FBlueprintEditorUtils::RenameGraphWithSuggestion(BoundGraph, NameValidator, TEXT("State"));

	//// Initialize the anim graph
	//const UEdGraphSchema* Schema = BoundGraph->GetSchema();
	//Schema->CreateDefaultNodesForGraph(*BoundGraph);

	//// Add the new graph as a child of our parent graph
	//UEdGraph* ParentGraph = GetGraph();

	//if (ParentGraph->SubGraphs.Find(BoundGraph) == INDEX_NONE)
	//{
	//	ParentGraph->SubGraphs.Add(BoundGraph);
	//}
}


void USCTGraphNode_Chapter::DestroyNode()
{
	/*UEdGraph* GraphToRemove = BoundGraph;*/

	//BoundGraph = NULL;
	Super::DestroyNode();

}



UEdGraphPin* USCTGraphNode_Chapter::GetInputPin() const
{
	return Pins[0];
}


UEdGraphPin* USCTGraphNode_Chapter::GetOutputPin() const
{
	return Pins[1];
}



#undef LOCTEXT_NAMESPACE