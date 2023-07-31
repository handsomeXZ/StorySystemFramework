#pragma once

#include "CoreMinimal.h"
#include "Manager/StoryDialogueTypes.h"
#include "Asset/StoryDialogueTree.h"

#include "DialogueStateMachine.generated.h"

DECLARE_DELEGATE_OneParam(FOnSelectOptionHandler, int32);
DECLARE_DELEGATE(FOnContinueContentDialogueHandler);


USTRUCT(BlueprintType)
struct FDialogueTaskContext
{
	GENERATED_BODY()
public:
	float DeltaSeconds;
	//////////////////////////////////////////////////////////////////////////
	FIndexHandle CurrentIndexHandle;
	bool bTimeWaitForSkip;
	float TimeWaitForSkip;

	FOnSelectOptionHandler SelectOptionDelegate;
	FOnContinueContentDialogueHandler ContinueContentDialogueDelegate;
	//////////////////////////////////////////////////////////////////////////
	FDialogueGlobalContext ActionContext;
	UPROPERTY();
	TObjectPtr<UStoryDialogueTree> StoryDialogueTree;
	TMap<TObjectPtr<UInputAction>, FInputHandle> InputHandles;
	FWorldContext WorldContext;
	//////////////////////////////////////////////////////////////////////////
	FOnReceiveHandler PlayerOnReceiveDelegate;
	FOnDiscardHandler PlayerOnDiscardDelegate;

	TMap<FGameplayTag, FOnReceiveHandler> ReceiverEventMap;
	TMap<FGameplayTag, FOnDiscardHandler> DiscardEventMap;
	// Contains references to prevent accidental GC.
	UPROPERTY();
	TMap<FGameplayTag, TObjectPtr<AActor>> ReceiverOwnerMap;
};

UCLASS()
class UDialogueStateBase : public UObject
{
	GENERATED_BODY()
public:
	EDialogueStateName StateID;

	virtual void Execute(FDialogueTaskContext& Context) {}
	virtual bool CanTransition(FDialogueTaskContext& Context) { return false; }
	virtual void Reset() {}
};

UCLASS()
class UDialogueStateMachineBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void Execute(FDialogueTaskContext& Context) {}

	virtual void Update(FDialogueTaskContext& Context) {}

	virtual void SwitchState(FDialogueTaskContext& Context, ESDTNodeType PrevNodeType, ESDTNodeType NextNodeType) {}

	virtual EDialogueStateName GetState() { return EDialogueStateName::UnKnown; }

	virtual void InActivateState(FDialogueTaskContext& Context) {}
};
//////////////////////////////////////////////////////////////////////////

class FDialogueStateFactory
{
public:
	static UDialogueStateBase* CreateState(UObject* Outer, EDialogueStateName StateName);
};

//////////////////////////////////////////////////////////////////////////
// DialogueStateMachine

UCLASS()
class UDialogueTaskStateMachine : public UDialogueStateMachineBase
{
	GENERATED_BODY()
public:
	UDialogueTaskStateMachine();

	virtual void Execute(FDialogueTaskContext& Context) override;

	virtual void Update(FDialogueTaskContext& Context) override;

	virtual void SwitchState(FDialogueTaskContext& Context, ESDTNodeType PrevNodeType, ESDTNodeType NextNodeType) override;

	virtual EDialogueStateName GetState() override;

	virtual void InActivateState(FDialogueTaskContext& Context) override;

private:
	UDialogueStateBase* CurrentState;
	ESDTNodeType PrevNodeType;
	ESDTNodeType CurrentNodeType;
	UPROPERTY()
	TMap<EDialogueStateName, UDialogueStateBase*> StateMap;
};

//////////////////////////////////////////////////////////////////////////
UCLASS()
class UDialogueState_InActive : public UDialogueStateBase
{
	GENERATED_BODY()
public:
	UDialogueState_InActive() { StateID = EDialogueStateName::InActive; }

	virtual void Execute(FDialogueTaskContext& Context) override {}
	virtual bool CanTransition(FDialogueTaskContext& Context) override;
};

UCLASS()
class UDialogueState_Active : public UDialogueStateBase
{
	GENERATED_BODY()
public:
	UDialogueState_Active() { StateID = EDialogueStateName::Active; }

	virtual void Execute(FDialogueTaskContext& Context) override;
	virtual bool CanTransition(FDialogueTaskContext& Context) override
	{
		Reset();
		return true;
	}
};

UCLASS()
class UDialogueState_Execute : public UDialogueStateBase
{
	GENERATED_BODY()
public:
	UDialogueState_Execute() { StateID = EDialogueStateName::Execute; }

	virtual void Execute(FDialogueTaskContext& Context) override;

	virtual bool CanTransition(FDialogueTaskContext& Context) override;

	virtual void Reset() override
	{
		ResultOfSelector = false;
	}

private:
	bool ResultOfSelector;
};

UCLASS()
class UDialogueState_WaitingSingleDialogue : public UDialogueStateBase
{
	GENERATED_BODY()
public:
	UDialogueState_WaitingSingleDialogue() { StateID = EDialogueStateName::WaitingSingleDialogue; }


	virtual void Execute(FDialogueTaskContext& Context) override;

	virtual bool CanTransition(FDialogueTaskContext& Context) override;

	virtual void Reset() override
	{
		bIsActive = false;
		bInputLocked = true;
	}

	void ActiveThisState();


private:
	bool bIsActive;
	bool bInputLocked;
};

UCLASS()
class UDialogueState_WaitingOptionsDialogue : public UDialogueStateBase
{
	GENERATED_BODY()
public:
	UDialogueState_WaitingOptionsDialogue() { StateID = EDialogueStateName::WaitingOptionsDialogue; }

	virtual void Execute(FDialogueTaskContext& Context) override;

	virtual bool CanTransition(FDialogueTaskContext& Context) override;

	virtual void Reset() override
	{
		bIsActive = false;
		SelectedOptionID = -1;
	}

	void ActiveThisState(int32 OptionID);

private:
	bool bIsActive;
	int32 SelectedOptionID;
};

UCLASS()
class UDialogueState_WaitingBlockingAction : public UDialogueStateBase
{
	GENERATED_BODY()
public:
	UDialogueState_WaitingBlockingAction() { StateID = EDialogueStateName::WaitingBlockingAction; }

	virtual void Execute(FDialogueTaskContext& Context) override;

	virtual bool CanTransition(FDialogueTaskContext& Context) override;

};

UCLASS()
class UDialogueState_Exit : public UDialogueStateBase
{
	GENERATED_BODY()
public:
	UDialogueState_Exit() { StateID = EDialogueStateName::Exit; }

	virtual void Execute(FDialogueTaskContext& Context) override;

	virtual bool CanTransition(FDialogueTaskContext& Context) override;
};

