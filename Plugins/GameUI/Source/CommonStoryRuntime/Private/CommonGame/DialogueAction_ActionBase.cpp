#include "CommonGame/DialogueAction_ActionBase.h"

UDialogueAction_ActionBase::UDialogueAction_ActionBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

UWorld* UDialogueAction_ActionBase::GetWorld() const
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