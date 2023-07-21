#pragma once

#include "CoreMinimal.h"
#include "Node/SDTNode.h"
#include "GameplayTagContainer.h"
#include "SDTNode_DContinue.generated.h"

class USDTDSelectorAction;

UCLASS()
class USDTNode_DContinue : public USDTNode
{
	GENERATED_BODY()
public:
	USDTNode_DContinue() : USDTNode(ESDTNodeType::Continue) {}

};