
#pragma once

#include "CoreMinimal.h"
#include "Node/SDTGraphNode.h"

#include "SDTGraphNode_DContent.generated.h"

class UObject;

UCLASS(MinimalAPI)
class USDTGraphNode_DContent : public USDTGraphNode
{
	GENERATED_BODY()
public:

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	// Support for copy, paste, cut, delete
	virtual bool CanDuplicateNode() const override { return true; }
	//~ End UEdGraphNode Interface

	//~ Begin USCTGraphNode Interface
	virtual UEdGraphPin* GetInputPin() const override;
	virtual UEdGraphPin* GetOutputPin() const override;
	virtual FString GetNodeName() const override;
	//~ End USCTGraphNode Interface

	FText GetContent() const;
};
