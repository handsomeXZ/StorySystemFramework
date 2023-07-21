#include "Manager/DialogueManager.h"

#include "CommonGame/DialogueTableRaw.h"
#include "PrimaryDataAsset/DialogueDataView.h"

#include "Manager/StoryChapterManager.h"
#include "GameUIManagerSubsystem.h"
#include "PrimaryGameLayout.h"
#include "GameUIPolicy.h"
#include "CommonLocalPlayer.h"

#include "TimerManager.h"
#include "PlayerMappableInputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"


UDialogueManager::UDialogueManager()
{
	Context.StoryDialogueTree = nullptr;
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		StateMachine = NewObject<UDialogueTaskStateMachine>(this, TEXT("DialogueTaskStateMachine"));
	}
}

void UDialogueManager::Tick(float DeltaTime)
{
	Context.DeltaSeconds = DeltaTime;
	if (StateMachine != nullptr)
	{
		StateMachine->Update(Context);
	}
}

TStatId UDialogueManager::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UDialogueManager, STATGROUP_Tickables);
}

ETickableTickType UDialogueManager::GetTickableTickType() const
{
	return (HasAnyFlags(RF_ClassDefaultObject))
		? ETickableTickType::Never
		: ETickableTickType::Always;
}

bool UDialogueManager::ActivateDialogueTask(FGameplayTag IdentityTag)
{
	if (StateMachine->GetState() != EDialogueStateName::InActive)
	{
		return false;
	}

	if (UStoryChapterManager* SCM = GetGameInstance()->GetSubsystem<UStoryChapterManager>())
	{
		if (UStoryDialogueTree* SDT = SCM->GetStoryDialogueTree(IdentityTag))
		{
			Context. StoryDialogueTree = SDT;
			Context.WorldContext = *GetGameInstance()->GetWorldContext();
			return true;
		}
	}

	return false;
}

bool UDialogueManager::RegisterForDialogueTask(FGameplayTag IdentityTag, AActor* Owner, FOnReceiveHandler ReceiveHandler, FOnDiscardHandler DiscardHandler)
{
	if (IdentityTag.IsValid())
	{
		Context.ReceiverEventMap.Add(IdentityTag, ReceiveHandler);
		Context.DiscardEventMap.Add(IdentityTag, DiscardHandler);
		Context.ReceiverOwnerMap.Add(IdentityTag, Owner);
		return true;
	}

	return false;
}

void UDialogueManager::UnRegisterForDialogueTask(FGameplayTag IdentityTag)
{
	Context.ReceiverOwnerMap.Remove(IdentityTag);
	Context.ReceiverEventMap.Remove(IdentityTag);
	Context.DiscardEventMap.Remove(IdentityTag);
}

void UDialogueManager::RegisterPlayer(FOnReceiveHandler PlayerOnReceiveDelegate, FOnDiscardHandler PlayerOnDiscardDelegate)
{
	Context.PlayerOnReceiveDelegate = PlayerOnReceiveDelegate;
	Context.PlayerOnDiscardDelegate = PlayerOnDiscardDelegate;
}

void UDialogueManager::UnRegisterPlayer()
{
	Context.PlayerOnReceiveDelegate.Unbind();
	Context.PlayerOnDiscardDelegate.Unbind();
}

void UDialogueManager::SelectDialogue(FIndexHandle OptionID)
{
	Context.SelectOptionDelegate.ExecuteIfBound(OptionID);
}

void UDialogueManager::ResetManager()
{
	UnBindInput();
}

void UDialogueManager::UnBindInput()
{
	APlayerController* controller = GetGameInstance()->GetWorld()->GetFirstPlayerController();
	check(controller);
	if (controller->InputComponent)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(controller->InputComponent))
		{
			if (UEnhancedInputLocalPlayerSubsystem* LPS = controller->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				for (auto& Elem : Context.InputHandles)
				{
					EnhancedInputComponent->RemoveBindingByHandle(Elem.Value.InputBindingHandle);
					LPS->RemovePlayerMappableConfig(Elem.Value.InputConfig);
				}
			}
		}
	}
}