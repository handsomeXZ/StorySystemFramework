#include "Asset/StoryDialogueTree.h"
#include "CommonGame/DialogueAction_RuleAction.h"

//////////////////////////////////////////////////////////////////////////

bool USDTDActionBase::CheckSelectedByBitInfo(int32 Index, uint8 SelectedBitInfo) const
{
	uint8 compare = FGenericPlatformMath::Exp2(float(Index));
	if ((compare & SelectedBitInfo) == compare)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
// USDTDSelectorAction

bool USDTDSelectorAction::Execute(FDialogueGlobalContext& Context, FWorldContext& WorldContext) const
{
	bool result = false;

	Context.bIsDirty = true;

	// Pass in a UWorld object to be used in case GetWorld() fails.
	FDialogueGlobalContext TempContext = Execute_BlueprintImplement(Context, WorldContext.World(), result);

	if (TempContext.bIsDirty == true)
	{
		Context = TempContext;
		Context.bIsDirty = false;
	}

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
		Context.bIsDirty = true;
		FDialogueGlobalContext  TempContext = Execute_BlueprintImplement(Context, WorldContext.World());
		if (TempContext.bIsDirty == true)
		{
			Context = TempContext;
			Context.bIsDirty = false;
		}
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