#include "CommonGame/DialogueAction_ActionBase.h"

UDialogueAction_ActionBase::UDialogueAction_ActionBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CacheWorld(nullptr)
{

}

void UDialogueAction_ActionBase::Initialize(const FWorldContext& WorldContext)
{
	CacheWorld = WorldContext.World();
}

UWorld* UDialogueAction_ActionBase::GetWorld() const
{
	return CacheWorld;
}