
#pragma once

#include "CoreMinimal.h"
#include "Node/SCTGraphNode.h"

#include "SCTGraphNode_Transition.generated.h"

class UObject;

UCLASS(MinimalAPI)
class USCTGraphNode_Transition : public USCTGraphNode
{
	GENERATED_UCLASS_BODY()
public:

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual void DestroyNode() override;
	//virtual TArray<UEdGraph*> GetSubGraphs() const override { return TArray<UEdGraph*>({ BoundGraph }); }
	//~ End UEdGraphNode Interface

	//~ Begin USCTGraphNode Interface
	virtual UEdGraphPin* GetInputPin() const override;
	virtual UEdGraphPin* GetOutputPin() const override;
	virtual FString GetNodeName() const override;
	//~ End USCTGraphNode Interface

	USCTGraphNode* GetPrevNode() const;
	USCTGraphNode* GetNextNode() const;
	void CreateConnections(USCTGraphNode* PrevNode, USCTGraphNode* NextNode);


	/*virtual UEdGraph* GetBoundGraph() const override { return BoundGraph; }
	virtual void ClearBoundGraph() override { BoundGraph = nullptr; }*/

public:
	// Allows a node to have its own diagram inside
	/*UPROPERTY()
	TObjectPtr<class UEdGraph> BoundGraph;*/
};
