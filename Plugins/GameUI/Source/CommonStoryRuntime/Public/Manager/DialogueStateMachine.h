#pragma once

#include "CoreMinimal.h"
#include "Manager/StoryDialogueTypes.h"
#include "Asset/StoryDialogueTree.h"

#include "DialogueStateMachine.generated.h"

DECLARE_DELEGATE_OneParam(FOnSelectOptionHandler, FIndexHandle);

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
	//////////////////////////////////////////////////////////////////////////
	TObjectPtr<UStoryDialogueTree> StoryDialogueTree;
	FWorldContext WorldContext;
	TMap<TObjectPtr<UInputAction>, FInputHandle> InputHandles;
	//////////////////////////////////////////////////////////////////////////
	FOnReceiveHandler PlayerOnReceiveDelegate;
	FOnDiscardHandler PlayerOnDiscardDelegate;

	FOnSelectOptionHandler SelectOptionDelegate;

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

	virtual void SwitchState(ESDTNodeType PrevNodeType, ESDTNodeType NextNodeType) {}

	virtual EDialogueStateName GetState() { return EDialogueStateName::UnKnown; }
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

	virtual void SwitchState(ESDTNodeType PrevNodeType, ESDTNodeType NextNodeType) override;

	virtual EDialogueStateName GetState() override;

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

	virtual void Execute(FDialogueTaskContext& Context) {}
	virtual bool CanTransition(FDialogueTaskContext& Context);
};

UCLASS()
class UDialogueState_Active : public UDialogueStateBase
{
	GENERATED_BODY()
public:
	UDialogueState_Active() { StateID = EDialogueStateName::Active; }

	virtual void Execute(FDialogueTaskContext& Context)
	{
		Context.CurrentIndexHandle = Context.StoryDialogueTree->RootIndex;
		Context.bTimeWaitForSkip = false;
	}
	virtual bool CanTransition(FDialogueTaskContext& Context)
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

	virtual void Execute(FDialogueTaskContext& Context);

	virtual bool CanTransition(FDialogueTaskContext& Context);

	virtual void Reset()
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


	virtual void Execute(FDialogueTaskContext& Context);

	virtual bool CanTransition(FDialogueTaskContext& Context);

	virtual void Reset()
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

	virtual void Execute(FDialogueTaskContext& Context);

	virtual bool CanTransition(FDialogueTaskContext& Context);

	virtual void Reset()
	{
		bIsActive = false;
		SelectedOptionID = FIndexHandle();
	}

	void ActiveThisState(FIndexHandle OptionID);

private:
	bool bIsActive;
	FIndexHandle SelectedOptionID;
};

UCLASS()
class UDialogueState_Exit : public UDialogueStateBase
{
	GENERATED_BODY()
public:
	UDialogueState_Exit() { StateID = EDialogueStateName::Exit; }

	virtual void Execute(FDialogueTaskContext& Context);

	virtual bool CanTransition(FDialogueTaskContext& Context);
};

