#pragma once

#include "CoreMinimal.h"
#include "Node/SDTNode.h"
#include "GameplayTagContainer.h"
#include "SDTNode_DAction_Instance.generated.h"

class USDTDCommonAction;
enum class ESDTCommonActionType : uint8;

UCLASS()
class USDTNode_DAction_Instance : public USDTNode
{
	GENERATED_BODY()
public:
	USDTNode_DAction_Instance() : USDTNode(ESDTNodeType::Action) {}

	UPROPERTY(EditAnywhere, Category = CommonAction)
	FString NodeName;
	UPROPERTY(EditAnywhere, Category = CommonAction)
	ESDTCommonActionType ActionType;
	UPROPERTY(Instanced,EditAnywhere, Category = CommonAction)
	TObjectPtr<USDTDCommonAction> ActionInstance;
};