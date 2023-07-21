#include "CommonGame/DialogueAction_SelectAction.h"
#include "Manager/StoryDialogueTypes.h"
#include "Manager/DialogueManager.h"

UDialogueAction_SelectAction::UDialogueAction_SelectAction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UDialogueAction_SelectAction::ExecuteAction(FStoryDialogueContext& Context) const
{
	ExecuteAction_BlueprintImplement(Context);
	// ...
}