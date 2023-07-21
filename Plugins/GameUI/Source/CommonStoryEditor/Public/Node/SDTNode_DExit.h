#pragma once

#include "CoreMinimal.h"
#include "Node/SDTNode.h"
#include "GameplayTagContainer.h"
#include "SDTNode_DExit.generated.h"


UCLASS()
class USDTNode_DExit : public USDTNode
{
	GENERATED_BODY()
public:
	USDTNode_DExit() : USDTNode(ESDTNodeType::Exit) {}

};