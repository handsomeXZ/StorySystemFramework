#pragma once

#include "CoreMinimal.h"
#include "Node/SDTGraphNode.h"
#include "Internationalization/Text.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"

#include "SDTGraphNode_DReturn.generated.h"

class UObject;

UCLASS(MinimalAPI)
class USDTGraphNode_DReturn : public USDTGraphNode
{
	GENERATED_BODY()
public:
	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetTooltipText() const override;
	//~ End UEdGraphNode Interface

	//~ Begin USCTGraphNode Interface
	virtual UEdGraphPin* GetInputPin() const override;
	//~ End USCTGraphNode Interface

	UEdGraphNode* GetOutputNode() const;
};
