#include "Node/SCTNode_Transition.h"


//////////////////////////////////////////////////////////////////////////
bool UTransitionAction::Execute()
{
	bool result = false;

	Execute_BlueprintImplement(result);


	return result;
}


UWorld* UTransitionAction::GetWorld() const
{
	auto& WorldContexts = GEngine->GetWorldContexts();
	for (const FWorldContext& context : WorldContexts)
	{
		if (context.WorldType == EWorldType::Game || context.WorldType == EWorldType::PIE)
		{
			return context.World();
		}
	}

	return nullptr;
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