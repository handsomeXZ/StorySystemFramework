#include "CommonGame/DialogueAction_RuleAction.h"
#include "Manager/StoryDialogueTypes.h"

void UDialogueAction_RuleAction::ExecuteAction(FStoryDialogueContext& Context, bool& RlueActionResult) const
{
	ExecuteAction_BlueprintImplement(Context, RlueActionResult);
	//...
}