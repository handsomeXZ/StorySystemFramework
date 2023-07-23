#pragma once

#include "CoreMinimal.h"
#include "Node/SDTNode.h"
#include "GameplayTagContainer.h"
#include "SDTNode_DSelector_Instance.generated.h"

class USDTDSelectorAction;

UCLASS()
class USDTNode_DSelector_Instance : public USDTNode
{
	GENERATED_BODY()
public:
	USDTNode_DSelector_Instance() : USDTNode(ESDTNodeType::Selector) {}

	UPROPERTY(Instanced, EditAnywhere, Category = Selector)
	TObjectPtr<USDTDSelectorAction> ActionInstance;
};