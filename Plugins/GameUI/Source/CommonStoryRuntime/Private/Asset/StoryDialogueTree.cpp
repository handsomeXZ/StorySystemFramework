#include "Asset/StoryDialogueTree.h"
#include "CommonGame/DialogueAction_RuleAction.h"

bool USDTDSelectorAction::Execute() const
{
	bool result = false;

	Execute_BlueprintImplement(result);


	return result;
}

void USDTDCommonAction::Execute() const
{
	Execute_BlueprintImplement();
}

UStoryDialogueTree::UStoryDialogueTree(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}