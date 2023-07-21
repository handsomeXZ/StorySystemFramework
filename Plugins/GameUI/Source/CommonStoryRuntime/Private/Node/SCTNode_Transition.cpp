#include "Node/SCTNode_Transition.h"


//////////////////////////////////////////////////////////////////////////
bool UTransitionAction::Execute()
{
	bool result = false;

	Execute_BlueprintImplement(result);


	return result;
}


//////////////////////////////////////////////////////////////////////////

USCTNode_Transition::USCTNode_Transition(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Action = nullptr;
}

bool USCTNode_Transition::ProcessTransition()
{
	if (bAutomaticTransition)
	{
		return true;
	}
	if (Action != nullptr)
	{
		return Action->Execute();
	}

	return false;
}