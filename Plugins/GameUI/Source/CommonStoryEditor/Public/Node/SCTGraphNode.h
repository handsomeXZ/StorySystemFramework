// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "SCTGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class COMMONSTORYEDITOR_API USCTGraphNode : public UEdGraphNode
{
	GENERATED_BODY()
public:
	/** instance class */
	UPROPERTY()
	UClass* NodeClass;

	UPROPERTY()
	TObjectPtr<UObject> NodeInstance;


	//~ Begin UEdGraphNode Interface
	virtual void PostPasteNode() override;
	virtual void PostPlacedNewNode() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual void NodeConnectionListChanged() override;
	virtual UObject* GetJumpTargetForDoubleClick() const override;
	virtual bool CanJumpToDefinition() const override;
	virtual void JumpToDefinition() const override;
	virtual void PrepareForCopying() override;
	virtual bool CanDuplicateNode() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual void DestroyNode() override;
	//~ End UEdGraphNode Interface

	// @return the input pin for this Node
	virtual UEdGraphPin* GetInputPin() const { return NULL; }

	// @return the output pin for this Node
	virtual UEdGraphPin* GetOutputPin() const { return NULL; }

	// @return the name of this Node
	virtual FString GetNodeName() const { return TEXT("SCTGraphNode"); }

	virtual UEdGraph* GetBoundGraph() const { return nullptr; }
	virtual void ClearBoundGraph() {}

	virtual void PostCopyNode();

	virtual void GetTransitionList(TArray<class USCTGraphNode_Transition*>& OutTransitions) const;

	/** initialize instance object  */
	virtual void InitializeNodeInstance();

protected:
	virtual void ResetNodeOwner();

};
