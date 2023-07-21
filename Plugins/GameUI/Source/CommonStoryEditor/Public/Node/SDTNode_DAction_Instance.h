#pragma once

#include "CoreMinimal.h"
#include "Node/SDTNode.h"
#include "GameplayTagContainer.h"
#include "SDTNode_DAction_Instance.generated.h"

class USDTDCommonAction;

UCLASS()
class USDTNode_DAction_Instance : public USDTNode
{
	GENERATED_BODY()
public:
	USDTNode_DAction_Instance() : USDTNode(ESDTNodeType::Action) {}

	UPROPERTY(Instanced,EditAnywhere, Category = CommonAction)
	TObjectPtr<USDTDCommonAction> ActionInstance;
};