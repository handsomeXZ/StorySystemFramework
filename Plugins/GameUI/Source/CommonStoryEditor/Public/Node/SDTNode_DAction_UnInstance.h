#pragma once

#include "CoreMinimal.h"
#include "Node/SDTNode.h"
#include "GameplayTagContainer.h"
#include "SDTNode_DAction_UnInstance.generated.h"

class USDTDCommonAction;

UCLASS()
class USDTNode_DAction_UnInstance : public USDTNode
{
	GENERATED_BODY()
public:
	USDTNode_DAction_UnInstance() : USDTNode(ESDTNodeType::Action) {}

	UPROPERTY(EditAnywhere, Category = CommonAction)
	TSubclassOf<USDTDCommonAction> ActionClass;
};