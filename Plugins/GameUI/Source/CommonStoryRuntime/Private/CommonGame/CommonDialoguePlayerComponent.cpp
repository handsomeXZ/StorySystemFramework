#include "CommonGame/CommonDialoguePlayerComponent.h"

#include "Manager/DialogueManager.h"
#include "Manager/StoryDialogueTypes.h"

#include "GameUIManagerSubsystem.h"
#include "CommonLocalPlayer.h"
#include "PrimaryGameLayout.h"
#include "GameUIPolicy.h"
#include "PlayerMappableInputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"

UCommonDialoguePlayerComponent::UCommonDialoguePlayerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PlayerDialogueOptionClass = nullptr;
	PlayerDialogueEntryClass = nullptr;
	LocalPlayerDialogueWidget = nullptr;
}

void UCommonDialoguePlayerComponent::Register()
{
	if (UDialogueManager* DM = UGameInstance::GetSubsystem<UDialogueManager>(GetWorld()->GetGameInstance()))
	{
		DM->RegisterPlayer(FOnReceiveHandler::CreateUObject(this, &ThisClass::OnReceiveDialogues), FOnDiscardHandler::CreateUObject(this, &ThisClass::Immediate_DiscardShowing));
	}
}

void UCommonDialoguePlayerComponent::UnRegister()
{
	if (UDialogueManager* DM = UGameInstance::GetSubsystem<UDialogueManager>(GetWorld()->GetGameInstance()))
	{
		DM->UnRegisterPlayer();
	}
}

void UCommonDialoguePlayerComponent::BeginPlay()
{
	Super::BeginPlay();
	Register();
}

void UCommonDialoguePlayerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UnRegister();
}


void UCommonDialoguePlayerComponent::OnReceiveDialogues(FStoryDialogueContext& Context)
{

	if (Context.DialogueType == EDialogueType::Multiple)
	{
		ShowDialogueOptionsBox(Context);
	}
	else
	{
		ShowDialogueBox(Context);
	}

}

void UCommonDialoguePlayerComponent::ShowDialogueBox(FStoryDialogueContext& Context)
{
	PushLocalPlayerDialogueWidgetToLayer(LocalPlayerDialogueWidget, PlayerDialogueEntryClass);
	if (ICommonDialogue_Interface* interface = Cast<ICommonDialogue_Interface>(LocalPlayerDialogueWidget))
	{
		interface->SetDialogueToShow(Context);
	}
}

void UCommonDialoguePlayerComponent::ShowDialogueOptionsBox(FStoryDialogueContext& Context)
{
	UDialogueManager* DialogueManager = UGameInstance::GetSubsystem<UDialogueManager>(GetWorld()->GetGameInstance());

	PushLocalPlayerDialogueWidgetToLayer(LocalPlayerDialogueWidget, PlayerDialogueOptionClass);
	if (ICommonDialogue_Interface* interface = Cast<ICommonDialogue_Interface>(LocalPlayerDialogueWidget))
	{
		interface->SetDialogueToShow(Context);
	}
}

void UCommonDialoguePlayerComponent::DiscardShowing(TObjectPtr<UCommonActivatableWidget>& LocalPlayerDialogueWidgetIn)
{
	if (LocalPlayerDialogueWidgetIn != nullptr)
	{
		if (UGameUIManagerSubsystem* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
		{
			if (UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
			{
				if (APlayerController* controller = GetController<APlayerController>())
				{
					if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(controller->GetLocalPlayer())))
					{
						RootLayout->FindAndRemoveWidgetFromLayer(LocalPlayerDialogueWidgetIn);
						LocalPlayerDialogueWidgetIn = nullptr;
					}
				}
			}
		}
	}
	
}

void UCommonDialoguePlayerComponent::PushLocalPlayerDialogueWidgetToLayer(TObjectPtr<UCommonActivatableWidget>& LocalPlayerDialogueWidgetIn, TSubclassOf<UCommonActivatableWidget> PlayerDialogueClassIn)
{
	if (LayerTag.IsValid() && PlayerDialogueClassIn)
	{
		if (UGameUIManagerSubsystem* UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UGameUIManagerSubsystem>())
		{
			if (UGameUIPolicy* Policy = UIManager->GetCurrentUIPolicy())
			{
				if (APlayerController* controller = GetController<APlayerController>())
				{
					if (UPrimaryGameLayout* RootLayout = Policy->GetRootLayout(CastChecked<UCommonLocalPlayer>(controller->GetLocalPlayer())))
					{
						LocalPlayerDialogueWidgetIn = RootLayout->PushWidgetToLayerStack(LayerTag, PlayerDialogueClassIn);
					}
				}
			}
		}
	}
}


void UCommonDialoguePlayerComponent::Immediate_DiscardShowing()
{
	//DiscardShowing(LocalPlayerDialogueOptionWidget);
	//DiscardShowing(LocalPlayerDialogueEntryWidget);
	DiscardShowing(LocalPlayerDialogueWidget);
}