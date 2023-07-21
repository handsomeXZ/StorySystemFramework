#include "FunctionLibraries/DialogueFuncLibrary.h"

#include "CommonGame/CommonDialogueActorComponent.h"
#include "CommonGame/IdentifyActorComponent.h"

#include "GameplayTagContainer.h"

bool UDialogueFuncLibrary::ActivateDialogueEvent(const UObject* ContextObject)
{
	if (ContextObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("ContextObject Find"));
		if (const AActor* actor = Cast<AActor>(ContextObject))
		{
			UE_LOG(LogTemp, Warning, TEXT("Actor Cast"));
			if (UCommonDialogueActorComponent* DialogueComp = Cast<UCommonDialogueActorComponent>(actor->FindComponentByClass(UCommonDialogueActorComponent::StaticClass())))
			{
				UE_LOG(LogTemp, Warning, TEXT("DialogueComp Cast"));
				DialogueComp->ActivateDialogueInteractive();
				return true;
			}
		}
	}

	return false;
}

FGameplayTag UDialogueFuncLibrary::GetTargetIdentify(const AActor* Target /* = nullptr */)
{
	if (Target)
	{
		if (UCommonDialogueActorComponent* DialogueComp = Cast<UCommonDialogueActorComponent>(Target->FindComponentByClass(UCommonDialogueActorComponent::StaticClass())))
		{
			return DialogueComp->IdentityTag;
		}
	}
	return FGameplayTag::EmptyTag;
}