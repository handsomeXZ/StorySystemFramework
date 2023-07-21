#pragma once

#include "CoreMinimal.h"
#include "Node/SDTNode.h"
#include "GameplayTagContainer.h"
#include "SDTNode_DReturn.generated.h"


UCLASS()
class USDTNode_DReturn : public USDTNode
{
	GENERATED_BODY()
public:
	USDTNode_DReturn() : USDTNode(ESDTNodeType::Return) {}

};