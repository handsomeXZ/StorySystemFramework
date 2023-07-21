#pragma once

#include "CoreMinimal.h"
#include "Node/SDTNode.h"
#include "GameplayTagContainer.h"
#include "SDTNode_DSelector_UnInstance.generated.h"

class USDTDSelectorAction;

UCLASS()
class USDTNode_DSelector_UnInstance : public USDTNode
{
	GENERATED_BODY()
public:
	USDTNode_DSelector_UnInstance() : USDTNode(ESDTNodeType::Selector) {}

	UPROPERTY(EditAnywhere, Category = Selector)
	TSubclassOf<USDTDSelectorAction> ActionClass;
};