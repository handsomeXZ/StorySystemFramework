// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "SDTGraphNode.generated.h"

/**
 *
 */
UCLASS()
class COMMONSTORYEDITOR_API USDTGraphNode : public UEdGraphNode
{
	GENERATED_BODY()
public:
	USDTGraphNode() : NodeClass(nullptr), ActionClass(nullptr) {}

	/** instance class */
	UPROPERTY()
	UClass* NodeClass;

	UPROPERTY()
	UClass* ActionClass;

	UPROPERTY()
	bool bInstancedAction;

	UPROPERTY()
	TObjectPtr<UObject> NodeInstance;

	//~ Begin UEdGraphNode Interface
	virtual void PostPlacedNewNode() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual void NodeConnectionListChanged() override;
	virtual void PrepareForCopying() override;
	virtual bool CanDuplicateNode() const override;
	virtual bool CanUserDeleteNode() const override;
	virtual void DestroyNode() override;
	//~ End UEdGraphNode Interface

	// @return the input pin for this Node
	virtual UEdGraphPin* GetInputPin() const { return nullptr; }

	// @return the output pin for this Node
	virtual UEdGraphPin* GetOutputPin() const { return nullptr; }

	// @return the name of this Node
	virtual FString GetNodeName() const { return TEXT("SDTGraphNode"); }

	virtual void PostCopyNode();

	/** initialize instance object  */
	virtual void InitializeNodeInstance();

protected:
	virtual void ResetNodeOwner();

};
