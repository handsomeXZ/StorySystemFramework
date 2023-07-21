#include "Node/SDTNode_DContent.h"


USDTNode_DContent::USDTNode_DContent()
	: USDTNode(ESDTNodeType::Content)
	, SourceMode(EDialogueSourceMode::NPC)
	, Content(FText::FromString(TEXT("empty")))
	, TimeWaitForSkip(-1)
{
	
}
