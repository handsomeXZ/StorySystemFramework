#pragma once

#include "CoreMinimal.h"
#include "Node/SDTNode.h"
#include "GameplayTagContainer.h"
#include "SDTNode_DOptions.generated.h"



USTRUCT(BlueprintType)
struct FSDTNode_DOptionItem
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FText Content;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDialogueAction_RuleAction> RuleAction;

	UPROPERTY(EditAnywhere)
	TMap<FName, float> ExtraFloat;
};

UCLASS()
class USDTNode_DOptions : public USDTNode
{
	GENERATED_BODY()
public:
	USDTNode_DOptions() : USDTNode(ESDTNodeType::Options) {}

	UPROPERTY(EditAnywhere, Category = Dialogue)
	TArray<FSDTNode_DOptionItem> Options;
};