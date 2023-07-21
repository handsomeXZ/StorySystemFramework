#pragma once

#include "CoreMinimal.h"
#include "Node/SCTGraphNode.h"
#include "Internationalization/Text.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"

#include "SCTGraphNode_Entry.generated.h"

class UObject;

UCLASS(MinimalAPI)
class USCTGraphNode_Entry : public USCTGraphNode
{
	GENERATED_UCLASS_BODY()


	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual bool CanUserDeleteNode() const override { return false; }
	virtual bool CanDuplicateNode() const override { return false; }
	//~ End UEdGraphNode Interface

	//~ Begin USCTGraphNode Interface
	virtual UEdGraphPin* GetOutputPin() const override;
	//~ End USCTGraphNode Interface

	UEdGraphNode* GetOutputNode() const;
};
