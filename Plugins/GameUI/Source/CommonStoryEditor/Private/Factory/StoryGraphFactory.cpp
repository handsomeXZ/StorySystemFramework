#include "Factory/StoryGraphFactory.h"

#include "SNode/SGraphNode_Chapter.h"
#include "SNode/SGraphNode_Entry.h"
#include "SNode/SGraphNode_Transition.h"
#include "SNode/SGraphNode_SDTEntry.h"
#include "SNode/SGraphNode_SDTExit.h"
#include "SNode/SGraphNode_SDTReturn.h"
#include "SNode/SGraphNode_SDTContinue.h"
#include "SNode/SGraphNode_SDTContent.h"
#include "SNode/SGraphNode_SDTOptions.h"
#include "SNode/SGraphNode_SDTSelector.h"
#include "SNode/SGraphNode_SDTAction.h"

#include "Node/SCTGraphNode_Chapter.h"
#include "Node/SCTGraphNode_Entry.h"
#include "Node/SCTGraphNode_Transition.h"
#include "Node/SDTGraphNode_DEntry.h"
#include "Node/SDTGraphNode_DExit.h"
#include "Node/SDTGraphNode_DReturn.h"
#include "Node/SDTGraphNode_DContinue.h"
#include "Node/SDTGraphNode_DContent.h"
#include "Node/SDTGraphNode_DOptions.h"
#include "Node/SDTGraphNode_DSelector.h"
#include "Node/SDTGraphNode_DAction.h"

#include "StoryChapterEdGraphSchema.h"
#include "Factory/SCTConnectionDrawingPolicy.h"

TSharedPtr<SGraphNode> FStoryNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	// SCT
	if (USCTGraphNode_Chapter* SCTNode = Cast<USCTGraphNode_Chapter>(Node))
	{
		return SNew(SGraphNode_Chapter, SCTNode);
	}
	
	if (USCTGraphNode_Entry* SCTNode = Cast<USCTGraphNode_Entry>(Node))
	{
		return SNew(SGraphNode_Entry, SCTNode);
	}
	
	if (USCTGraphNode_Transition* SCTNode = Cast<USCTGraphNode_Transition>(Node))
	{
		return SNew(SGraphNode_Transition, SCTNode);
	}

	//SDT
	if (USDTGraphNode_DEntry* SDTNode = Cast<USDTGraphNode_DEntry>(Node))
	{
		return SNew(SGraphNode_SDTEntry, SDTNode);
	}

	if (USDTGraphNode_DExit* SDTNode = Cast<USDTGraphNode_DExit>(Node))
	{
		return SNew(SGraphNode_SDTExit, SDTNode);
	}

	if (USDTGraphNode_DReturn* SDTNode = Cast<USDTGraphNode_DReturn>(Node))
	{
		return SNew(SGraphNode_SDTReturn, SDTNode);
	}

	if (USDTGraphNode_DContinue* SDTNode = Cast<USDTGraphNode_DContinue>(Node))
	{
		return SNew(SGraphNode_SDTContinue, SDTNode);
	}

	if (USDTGraphNode_DContent* SDTNode = Cast<USDTGraphNode_DContent>(Node))
	{
		return SNew(SGraphNode_SDTContent, SDTNode);
	}

	if (USDTGraphNode_DOptions* SDTNode = Cast<USDTGraphNode_DOptions>(Node))
	{
		return SNew(SGraphNode_SDTOptions, SDTNode);
	}

	if (USDTGraphNode_DSelector* SDTNode = Cast<USDTGraphNode_DSelector>(Node))
	{
		return SNew(SGraphNode_SDTSelector, SDTNode);
	}

	if (USDTGraphNode_DAction* SDTNode = Cast<USDTGraphNode_DAction>(Node))
	{
		return SNew(SGraphNode_SDTAction, SDTNode);
	}

	return nullptr;
}

FConnectionDrawingPolicy* FStoryPinConnectionFactory::CreateConnectionPolicy(const class UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const class FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
{
	if (Schema->IsA(UStoryChapterEdGraphSchema::StaticClass()))
	{
		return new FSCTConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements, InGraphObj);
	}

	return nullptr;
}