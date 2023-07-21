
#include "CommonGame/CommonDialogueActorComponent.h"
#include "CommonGame/CommonDialogue_WidgetComponent.h"

#include "Manager/StoryChapterManager.h"

#include "Components/WidgetComponent.h"



UCommonDialogueActorComponent::UCommonDialogueActorComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCommonDialogueActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCommonDialogueActorComponent::Initialize(FGameplayTag InIdentityTag, TSubclassOf<UCommonDialogue_WidgetComponent> InWidgetCompClass, TSubclassOf<UUserWidget> InWidgetClass
	, FVector InRelativeLocationIn, bool InbOverrideSizeIn, FVector2D InSizeOverrideIn)
{
	WidgetComp = NewObject<UCommonDialogue_WidgetComponent>(GetOwner(), InWidgetCompClass);
	IdentityTag = InIdentityTag;
	RelativeLocation = InRelativeLocationIn;
	bOverrideSize = InbOverrideSizeIn;
	SizeOverride = InSizeOverrideIn;

	WidgetComp->SetWidgetClass(WidgetClass);
	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	if (bOverrideSize)
	{
		WidgetComp->SetDrawSize(SizeOverride);
	}
	else
	{
		WidgetComp->SetDrawSize(FVector2D(0.0, 0.0));
	}
	WidgetComp->SetRelativeLocation(RelativeLocation);
	// GetOwner()->AddOwnedComponent(WidgetComp);
	WidgetComp->SetupAttachment(GetOwner()->GetRootComponent());
	WidgetComp->RegisterComponent();
}

void UCommonDialogueActorComponent::BeginPlay()
{
	Super::BeginPlay();
	Register();
	Initialize(IdentityTag, WidgetCompClass, WidgetClass, RelativeLocation, bOverrideSize, SizeOverride);
}

void UCommonDialogueActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UnRegister();
}


void UCommonDialogueActorComponent::ActivateDialogueInteractive()
{
	if (UDialogueManager* DM = UGameInstance::GetSubsystem<UDialogueManager>(GetWorld()->GetGameInstance()))
	{
		DM->ActivateDialogueTask(IdentityTag);
	}
}

void UCommonDialogueActorComponent::Register()
{
	if (UDialogueManager* DM = UGameInstance::GetSubsystem<UDialogueManager>(GetWorld()->GetGameInstance()))
	{
		DM->RegisterForDialogueTask(IdentityTag, GetOwner()
		, FOnReceiveHandler::CreateUObject(this, &ThisClass::OnReceiveDialogues)
		, FOnDiscardHandler::CreateUObject(this, &ThisClass::Immediate_DiscardShowing));
	}
}

void UCommonDialogueActorComponent::UnRegister()
{
	if (const UWorld* WorldPtr = GetWorld())
	{
		if (const UGameInstance* GameInstancePtr = WorldPtr->GetGameInstance())
		{
			if (UDialogueManager* DM = UGameInstance::GetSubsystem<UDialogueManager>(GameInstancePtr))
			{
				DM->UnRegisterForDialogueTask(IdentityTag);
			}
		}
	}
}

void UCommonDialogueActorComponent::OnReceiveDialogues(FStoryDialogueContext& Context)
{
	ShowTemporarily(Context);
}

void UCommonDialogueActorComponent::ShowTemporarily(FStoryDialogueContext& Context)
{
	WidgetComp->ShowTemporarily(Context);
}

void UCommonDialogueActorComponent::Immediate_DiscardShowing()
{
	WidgetComp->Immediate_DiscardShowing();
}
