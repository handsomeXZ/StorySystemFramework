#pragma once

#include "CoreMinimal.h"
#include "Node/SDTNode.h"
#include "GameplayTagContainer.h"
#include "Manager/DialogueManager.h"
#include "SDTNode_DContent.generated.h"


UCLASS()
class USDTNode_DContent : public USDTNode
{
	GENERATED_BODY()

public:
	USDTNode_DContent();

	UPROPERTY(EditAnywhere, Category = Dialogue)
	EDialogueSourceMode SourceMode;
	UPROPERTY(EditAnywhere, Category = Dialogue, meta = (EditCondition = "SourceMode == EDialogueSourceMode::NPC"))
	FGameplayTag IdentityTag;
	UPROPERTY(EditAnywhere, Category = Dialogue)
	FText Content;
	UPROPERTY(EditAnywhere, Category = Dialogue, meta = (EditCondition = "SourceMode == EDialogueSourceMode::NPC"))
	float TimeWaitForSkip;
};