#include "Node/SDTNode_DOptions.h"
#include "CommonGame\DialogueAction_RuleAction.h"

void USDTNode_DOptions::GetPersistObjects(TSet<UObject*>& Objects)
{
	for (FSDTNode_DOptionItem& item : Options)
	{
		if (item.RuleAction)
		{
			Objects.Add(item.RuleAction);
		}
	}
}
