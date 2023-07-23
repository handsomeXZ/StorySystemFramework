#pragma once

#include "CoreMinimal.h"
#include "Node/SCTNode.h"
#include "SCTNode_Transition.generated.h"

UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable, BlueprintType, Abstract)
class COMMONSTORYRUNTIME_API UTransitionAction : public UObject
{
	GENERATED_BODY()
public:
	virtual class UWorld* GetWorld() const override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Execute"))
	void Execute_BlueprintImplement(bool& Result);

	virtual bool Execute();
};

UCLASS(Blueprintable)
class COMMONSTORYRUNTIME_API USCTNode_Transition : public USCTNode
{
	GENERATED_UCLASS_BODY()
public:
	bool ProcessTransition();


public:
	UPROPERTY(EditAnywhere, Category = Transition)
	bool bAutomaticTransition;

	// The priority order of this transition. If multiple transitions out of a Node go
	// true at the same time, the one with the smallest priority order will take precedent
	UPROPERTY(EditAnywhere, Category=Transition)
	int32 PriorityOrder;

	UPROPERTY(EditAnywhere, Category = TransitionAction)
	TObjectPtr<UTransitionAction> Action;
};