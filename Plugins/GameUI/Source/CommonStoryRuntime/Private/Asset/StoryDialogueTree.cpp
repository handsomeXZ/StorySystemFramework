#include "Asset/StoryDialogueTree.h"
#include "CommonGame/DialogueAction_RuleAction.h"

//////////////////////////////////////////////////////////////////////////
// USDTDSelectorAction

bool USDTDSelectorAction::Execute(FDialogueGlobalContext& Context, FWorldContext& WorldContext) const
{
	bool result = false;

	// Pass in a UWorld object to be used in case GetWorld() fails.
	Context = Execute_BlueprintImplement(Context, WorldContext.World(), result);

	return result;
}

UWorld* USDTDSelectorAction::GetWorld() const
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
// USDTDCommonAction

void USDTDCommonAction::ContinueExc(FDialogueGlobalContext Context) const
{
	if (Context.ActionToContinue)
	{
		Context.ActionToContinue->ContinueExc();
	}
}

void USDTDCommonAction::Execute(FDialogueGlobalContext& Context, FWorldContext& WorldContext) const
{
	// Pass in a UWorld object to be used in case GetWorld() fails.
	if (Context.ActionToContinue != nullptr)
	{
		ExecuteAndBlocking_BlueprintImplement(Context, WorldContext.World());
	}
	else
	{
		Context = Execute_BlueprintImplement(Context, WorldContext.World());
	}

}

UWorld* USDTDCommonAction::GetWorld() const
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

UStoryDialogueTree::UStoryDialogueTree(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}