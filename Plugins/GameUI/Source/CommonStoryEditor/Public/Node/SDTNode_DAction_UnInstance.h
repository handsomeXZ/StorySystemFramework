#pragma once

#include "CoreMinimal.h"
#include "Node/SDTNode.h"
#include "GameplayTagContainer.h"
#include "SDTNode_DAction_UnInstance.generated.h"

class USDTDCommonAction;
enum class ESDTCommonActionType : uint8;

UCLASS()
class USDTNode_DAction_UnInstance : public USDTNode
{
	GENERATED_BODY()
public:
	USDTNode_DAction_UnInstance() : USDTNode(ESDTNodeType::Action), NodeName(FString()){}

	UPROPERTY(EditAnywhere, Category = CommonAction)
	FString NodeName;
	UPROPERTY(EditAnywhere, Category = CommonAction)
	ESDTCommonActionType ActionType;
	UPROPERTY(EditAnywhere, Category = CommonAction)
	TSubclassOf<USDTDCommonAction> ActionClass;
};