#pragma once

#include "CoreMinimal.h"
#include "Node/SDTGraphNode.h"
#include "Internationalization/Text.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"

#include "SDTGraphNode_DEntry.generated.h"

class UObject;

UCLASS(MinimalAPI)
class USDTGraphNode_DEntry : public USDTGraphNode
{
	GENERATED_BODY()
public:

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetTooltipText() const override;
	virtual bool CanUserDeleteNode() const override { return false; }
	virtual bool CanDuplicateNode() const override { return false; }
	//~ End UEdGraphNode Interface

	//~ Begin USCTGraphNode Interface
	virtual UEdGraphPin* GetOutputPin() const override;
	//~ End USCTGraphNode Interface

	UEdGraphNode* GetOutputNode() const;
};
