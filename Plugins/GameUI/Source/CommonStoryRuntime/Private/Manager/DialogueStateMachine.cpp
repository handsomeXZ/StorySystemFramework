#include "Manager/DialogueStateMachine.h"

#include "CommonGame/DialogueAction_RuleAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

UDialogueStateBase* FDialogueStateFactory::CreateState(UObject* Outer, EDialogueStateName StateName)
{
	switch (StateName)
	{
	case EDialogueStateName::InActive:					return NewObject<UDialogueState_InActive>(Outer, TEXT("DialogueState_InActive"));
	case EDialogueStateName::Active:					return NewObject<UDialogueState_Active>(Outer, TEXT("DialogueState_Active"));
	case EDialogueStateName::Execute:					return NewObject<UDialogueState_Execute>(Outer, TEXT("DialogueState_Execute"));
	case EDialogueStateName::WaitingSingleDialogue:		return NewObject<UDialogueState_WaitingSingleDialogue>(Outer, TEXT("DialogueState_WaitingSingleDialogue"));
	case EDialogueStateName::WaitingOptionsDialogue:	return NewObject<UDialogueState_WaitingOptionsDialogue>(Outer, TEXT("DialogueState_WaitingOptionsDialogue"));
	case EDialogueStateName::WaitingBlockingAction:		return NewObject<UDialogueState_WaitingBlockingAction>(Outer, TEXT("DialogueState_WaitingBlockingAction"));
	case EDialogueStateName::Exit:						return NewObject<UDialogueState_Exit>(Outer, TEXT("DialogueState_Exit"));
	}

	return nullptr;
}
//////////////////////////////////////////////////////////////////////////
// UDialogueTaskStateMachine

UDialogueTaskStateMachine::UDialogueTaskStateMachine()
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		StateMap.Add(EDialogueStateName::InActive, FDialogueStateFactory::CreateState(this, EDialogueStateName::InActive));
		StateMap.Add(EDialogueStateName::Active, FDialogueStateFactory::CreateState(this, EDialogueStateName::Active));
		StateMap.Add(EDialogueStateName::Execute, FDialogueStateFactory::CreateState(this, EDialogueStateName::Execute));
		StateMap.Add(EDialogueStateName::WaitingSingleDialogue, FDialogueStateFactory::CreateState(this, EDialogueStateName::WaitingSingleDialogue));
		StateMap.Add(EDialogueStateName::WaitingOptionsDialogue, FDialogueStateFactory::CreateState(this, EDialogueStateName::WaitingOptionsDialogue));
		StateMap.Add(EDialogueStateName::WaitingBlockingAction, FDialogueStateFactory::CreateState(this, EDialogueStateName::WaitingBlockingAction));
		StateMap.Add(EDialogueStateName::Exit, FDialogueStateFactory::CreateState(this, EDialogueStateName::Exit));

		PrevNodeType = ESDTNodeType::UnKnown;
		CurrentNodeType = ESDTNodeType::UnKnown;
		CurrentState = StateMap[EDialogueStateName::InActive];
	}
}

void UDialogueTaskStateMachine::Execute(FDialogueTaskContext& Context)
{
	if (CurrentState)
	{
		CurrentState->Execute(Context);
	}
}

void UDialogueTaskStateMachine::Update(FDialogueTaskContext& Context)
{
	
	if (CurrentState != nullptr && CurrentState->CanTransition(Context))
	{
		PrevNodeType = CurrentNodeType;
		CurrentNodeType = Context.CurrentIndexHandle.NodeType;
		SwitchState(Context, PrevNodeType, CurrentNodeType);
		// Execute immediately after the state Switch
		Execute(Context);
	}
}

void UDialogueTaskStateMachine::SwitchState(FDialogueTaskContext& Context, ESDTNodeType PrevNodeTypeIn, ESDTNodeType NextNodeTypeIn)
{
	// 没有更换状态则在原状态重新执行流程

	switch (CurrentState->StateID)
	{
	case EDialogueStateName::InActive:
	{
		CurrentState = StateMap[EDialogueStateName::Active];
		break;
	}
	case EDialogueStateName::Active:
	{
		CurrentState = StateMap[EDialogueStateName::Execute];
		break;
	}
	case EDialogueStateName::Execute:
	{
		if (PrevNodeTypeIn == ESDTNodeType::Content)
		{
			CurrentState = StateMap[EDialogueStateName::WaitingSingleDialogue];
		}
		else if (PrevNodeTypeIn == ESDTNodeType::Options)
		{
			CurrentState = StateMap[EDialogueStateName::WaitingOptionsDialogue];
		}
		else if (PrevNodeTypeIn == ESDTNodeType::Action && Context.ActionContext.ActionToContinue != nullptr)
		{
			// 阻塞对话流程，从原本的 -> `Execute` 转为 -> `WaitingBlockingAction` -> `Execute`
			CurrentState = StateMap[EDialogueStateName::WaitingBlockingAction];
		}
		else if (NextNodeTypeIn == ESDTNodeType::Exit)
		{
			CurrentState = StateMap[EDialogueStateName::Exit];
		}

		break;
	}
	case EDialogueStateName::Exit:
	{
		CurrentState = StateMap[EDialogueStateName::InActive];
		break;
	}
	default:
	{
		if (NextNodeTypeIn == ESDTNodeType::Exit)
		{
			CurrentState = StateMap[EDialogueStateName::Exit];
		}
		else
		{
			// 当前状态不在 switch() 所列出的状态里，则下一次自动转为 `Execute`
			CurrentState = StateMap[EDialogueStateName::Execute];
		}
		break;
	}
	}



}

EDialogueStateName UDialogueTaskStateMachine::GetState()
{
	if (CurrentState)
	{
		return CurrentState->StateID;
	}

	return EDialogueStateName::UnKnown;
}

void UDialogueTaskStateMachine::InActivateState(FDialogueTaskContext& Context)
{
	CurrentState = StateMap[EDialogueStateName::Exit];
	CurrentState->Execute(Context);
}

//////////////////////////////////////////////////////////////////////////
// UDialogueState_InActive

bool UDialogueState_InActive::CanTransition(FDialogueTaskContext& Context)
{
	if (Context.StoryDialogueTree != nullptr && Context.PlayerOnReceiveDelegate.IsBound() && Context.PlayerOnDiscardDelegate.IsBound())
	{
		if (Context.StoryDialogueTree->bReliable)
		{
			for (FGameplayTag& tag : Context.StoryDialogueTree->IdentifyTagList)
			{
				if (!Context.ReceiverOwnerMap.Find(tag) || !Context.ReceiverEventMap.Find(tag))
				{
					Context.StoryDialogueTree = nullptr;
					return false;
				}
			}
		}

		Reset();
		return true;
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////
// UDialogueState_Active

void UDialogueState_Active::Execute(FDialogueTaskContext& Context)
{
	Context.CurrentIndexHandle = Context.StoryDialogueTree->RootIndex;
	Context.bTimeWaitForSkip = false;
	for (FGameplayTag& tag : Context.StoryDialogueTree->IdentifyTagList)
	{
		if (TObjectPtr<AActor>* target = Context.ReceiverOwnerMap.Find(tag))
		{
			Context.ActionContext.Targets.Add(tag, *target);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// UDialogueState_Execute

void UDialogueState_Execute::Execute(FDialogueTaskContext& Context)
{
	FIndexHandle& IndexHandle = Context.CurrentIndexHandle;
	switch (IndexHandle.NodeType)
	{
	case ESDTNodeType::Content:
	{
		FSDTDContentNode& Node = Context.StoryDialogueTree->ContentNodeList[IndexHandle.Index];

		FStoryDialogueContext MyContext;
		MyContext.DialogueType = EDialogueType::Single;

		if (Node.SourceMode == EDialogueSourceMode::Player)
		{
			MyContext.Contents.Add(FDialogueItem(0, Node.Content));

			Context.PlayerOnReceiveDelegate.ExecuteIfBound(MyContext);
		}
		else
		{
			if (const FOnReceiveHandler* handle = Context.ReceiverEventMap.Find(Node.IdentityTag))
			{
				if (Node.TimeWaitForSkip > 0)
				{
					Context.bTimeWaitForSkip = true;
					Context.TimeWaitForSkip = Node.TimeWaitForSkip;
				}
				MyContext.Contents.Add(FDialogueItem(0, Node.Content));

				handle->ExecuteIfBound(MyContext);
			}
			else
			{
				// If the corresponding Receiver is not found and exceptions are not avoided, the system uses a TimeOut mechanism to jump to the next node
				Context.bTimeWaitForSkip = true;
				Context.TimeWaitForSkip = 0;
			}
		}

		break;
	}
	case ESDTNodeType::Options:
	{
		// Use bit to store selected information for up to 128 options
		// SelectedBit
		uint8 SelectedBit = 0x00;
		
		FSDTDOptionsNode& Node = Context.StoryDialogueTree->OptionsNodeList[IndexHandle.Index];
		FStoryDialogueContext MyContext;
		MyContext.DialogueType = EDialogueType::Multiple;

		int32 Index = 0;
		for (FSDTDOptionContainer& Container : Node.Options)
		{
			bool RlueResult = true;
			if (Container.OptionItem.RuleAction)
			{
				if (UDialogueAction_RuleAction* Action = Container.OptionItem.RuleAction)
				{
					Action->ExecuteAction(MyContext, RlueResult);
				}

			}

			if (RlueResult)
			{
				Context.ActionContext.FloatData.Append(Container.OptionItem.ExtraFloat);

				MyContext.Contents.Add(FDialogueItem(Index, Container.OptionItem.Content, Container.OptionItem.ExtraFloat, Container.OptionItem.bSelected));
			}

			if (Container.OptionItem.bSelected)
			{
				// SelectedBit
				SelectedBit += 0x01 * FGenericPlatformMath::Exp2(float(Index));
			}

			Index++;
		}

		// uin8  : 8bit
		// float : 32bit
		// SelectedBit
		float& Bit = Context.ActionContext.FloatData.FindOrAdd(FName(TEXT("SelectedBit")));
		Bit = SelectedBit;

		Context.PlayerOnReceiveDelegate.ExecuteIfBound(MyContext);

		break;
	}
	case ESDTNodeType::Selector:
	{
		FSDTDSelectorNode& Node = Context.StoryDialogueTree->SelectorNodeList[IndexHandle.Index];
		if (Node.SelectorInstance)
		{
			ResultOfSelector = Node.SelectorInstance->Execute(Context.ActionContext, Context.WorldContext);
		}
		else if (Node.SelectorClass)
		{
			// CDO
			const USDTDSelectorAction* Action = GetDefault<USDTDSelectorAction>(Node.SelectorClass);
			ResultOfSelector = Action->Execute(Context.ActionContext, Context.WorldContext);
		}
		break;
	}
	case ESDTNodeType::Action:
	{
		FSDTDActionNode& Node = Context.StoryDialogueTree->ActionNodeList[IndexHandle.Index];

		Context.ActionContext.ActionToContinue = nullptr;
		if (Node.ActionType == ESDTCommonActionType::Blocking)
		{
			UActionToContinue* ActionToContinue = NewObject<UActionToContinue>(this, UActionToContinue::StaticClass());
			Context.ActionContext.ActionToContinue = ActionToContinue;
		}

		if (Node.CommonActionInstance)
		{
			Node.CommonActionInstance->Execute(Context.ActionContext, Context.WorldContext);
		}
		else if (Node.CommonActionClass)
		{
			// CDO
			const USDTDCommonAction* Action = GetDefault<USDTDCommonAction>(Node.CommonActionClass);
			Action->Execute(Context.ActionContext, Context.WorldContext);
		}

		break;
	}
	}
}

bool UDialogueState_Execute::CanTransition(FDialogueTaskContext& Context)
{
	UE_LOG(LogTemp, Warning, TEXT("Check CanTransition Dialogue State: Execute"));
	FIndexHandle& IndexHandle = Context.CurrentIndexHandle;
	switch (IndexHandle.NodeType)
	{
	case ESDTNodeType::Action:
	{
		FSDTDActionNode& Node = Context.StoryDialogueTree->ActionNodeList[IndexHandle.Index];
		Context.CurrentIndexHandle = Node.Child;

		break;
	}
	case ESDTNodeType::Selector:
	{
		FSDTDSelectorNode& Node = Context.StoryDialogueTree->SelectorNodeList[IndexHandle.Index];
		Context.CurrentIndexHandle = ResultOfSelector ? Node.TrueIndex : Node.FalseIndex;

		break;
	}
	case ESDTNodeType::Continue:
	{
		FSDTDReturnNode& Node = Context.StoryDialogueTree->ReturnNodeList[IndexHandle.Index];
		Context.CurrentIndexHandle = Node.Child;

		break;
	}
	case ESDTNodeType::Return:
	{
		FSDTDReturnNode& Node = Context.StoryDialogueTree->ReturnNodeList[IndexHandle.Index];
		Context.CurrentIndexHandle = Node.Child;

		break;
	}
	}

	Reset();
	return true;

}

//////////////////////////////////////////////////////////////////////////
// UDialogueState_WaitingSingleDialogue

void UDialogueState_WaitingSingleDialogue::Execute(FDialogueTaskContext& Context)
{
	// No special skip processing is done here, when the default Input mode is found to be not provided, 
	// it will be handled by "UDialogueManager::ContinueContentDialogue()" and "TimeWaitForSkip". 
	/*bIsActive = (Context.StoryDialogueTree->InputAction) ? false : true;*/
	bIsActive = false;


	// Can force skip this state
	Context.ContinueContentDialogueDelegate.BindUObject(this, &ThisClass::ActiveThisState);


	bInputLocked = false;
	if (FInputHandle* handle = Context.InputHandles.Find(Context.StoryDialogueTree->InputAction))
	{
		return;
	}
	
	// By default, the first player Index is 0
	APlayerController* controller = Context.WorldContext.World()->GetFirstPlayerController();
	check(controller);
	if (controller->InputComponent)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(controller->InputComponent))
		{
			if (UEnhancedInputLocalPlayerSubsystem* LPS = controller->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				FInputHandle& handle = Context.InputHandles.FindOrAdd(Context.StoryDialogueTree->InputAction);

				handle.InputBindingHandle = EnhancedInputComponent->BindAction(Context.StoryDialogueTree->InputAction, ETriggerEvent::Started, this, &ThisClass::ActiveThisState).GetHandle();
				handle.InputConfig = Context.StoryDialogueTree->InputConfig;

				LPS->AddPlayerMappableConfig(Context.StoryDialogueTree->InputConfig);
			}
		}
	}
}

void UDialogueState_WaitingSingleDialogue::ActiveThisState()
{
	// Cannot be activated while locked
	bIsActive = !bInputLocked;
}

bool UDialogueState_WaitingSingleDialogue::CanTransition(FDialogueTaskContext& Context)
{
	bool Result = false;

	// Time Wait For Skip
	if (Context.bTimeWaitForSkip)
	{
		Context.TimeWaitForSkip -= Context.DeltaSeconds;
		if (Context.TimeWaitForSkip <= 0)
		{
			bIsActive = false;
			bInputLocked = true;

			Result = true;
		}
	}

	// Input Wait for Skip
	if (bIsActive)
	{
		bIsActive = false;
		bInputLocked = true;
		
		Result = true;
	}

	if (Result)
	{
		FIndexHandle& IndexHandle = Context.CurrentIndexHandle;
		FSDTDContentNode& Node = Context.StoryDialogueTree->ContentNodeList[IndexHandle.Index];

		if (Node.SourceMode == EDialogueSourceMode::Player)
		{
			Context.PlayerOnDiscardDelegate.Execute();
		}
		else if (const FOnDiscardHandler* handle = Context.DiscardEventMap.Find(Node.IdentityTag))
		{
			handle->Execute();
		}

		Context.CurrentIndexHandle = Node.Child;

		Context.ContinueContentDialogueDelegate.Unbind();
		Reset();
	}


	return Result;
}

//////////////////////////////////////////////////////////////////////////
// UDialogueState_WaitingOptionsDialogue
void UDialogueState_WaitingOptionsDialogue::Execute(FDialogueTaskContext& Context)
{
	bIsActive = false;

	Context.SelectOptionDelegate.BindUObject(this, &ThisClass::ActiveThisState);
}

void UDialogueState_WaitingOptionsDialogue::ActiveThisState(int32 OptionID)
{
	SelectedOptionID = OptionID;
	bIsActive = true;
}

bool UDialogueState_WaitingOptionsDialogue::CanTransition(FDialogueTaskContext& Context)
{
	if (bIsActive)
	{
		FIndexHandle IndexHandle = Context.CurrentIndexHandle;
		FSDTDOptionsNode& Node = Context.StoryDialogueTree->OptionsNodeList[IndexHandle.Index];
		FSDTDOptionContainer& Item = Node.Options[SelectedOptionID];

		Item.OptionItem.bSelected = true;
		Context.PlayerOnDiscardDelegate.Execute();
		Context.CurrentIndexHandle = Item.IndexHandle;

		Context.SelectOptionDelegate.Unbind();
		Reset();
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// UDialogueState_WaitingBlockingAction

void UDialogueState_WaitingBlockingAction::Execute(FDialogueTaskContext& Context)
{
	
}

bool UDialogueState_WaitingBlockingAction::CanTransition(FDialogueTaskContext& Context)
{
	if (Context.ActionContext.ActionToContinue->CanContinue())
	{
		Context.ActionContext.ActionToContinue = nullptr;
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// UDialogueState_Exit

void UDialogueState_Exit::Execute(FDialogueTaskContext& Context)
{

// Context Clear
	Context.CurrentIndexHandle.Index = -1;
	Context.CurrentIndexHandle.NodeType = ESDTNodeType::UnKnown;

	Context.bTimeWaitForSkip = false;
	Context.TimeWaitForSkip = 0;
	
	Context.SelectOptionDelegate.Unbind();
	Context.ContinueContentDialogueDelegate.Unbind();

	Context.ActionContext.FloatData.Empty();
	Context.ActionContext.Targets.Empty();
	Context.ActionContext.ActionToContinue = nullptr;
	Context.ActionContext.bIsDirty = false;

	// UnBind Input For Player, By default, the first player Index is 0.
	APlayerController* controller = Context.WorldContext.World()->GetFirstPlayerController();
	check(controller);
	if (controller->InputComponent)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(controller->InputComponent))
		{
			if (UEnhancedInputLocalPlayerSubsystem* LPS = controller->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				if (FInputHandle* handle = Context.InputHandles.Find(Context.StoryDialogueTree->InputAction))
				{
					EnhancedInputComponent->RemoveBindingByHandle(handle->InputBindingHandle);
					LPS->RemovePlayerMappableConfig(handle->InputConfig);
				}
			}
		}
	}
	Context.InputHandles.Empty();

	Context.WorldContext = FWorldContext();

	Context.StoryDialogueTree = nullptr;

// Discard All
	Context.PlayerOnDiscardDelegate.ExecuteIfBound();
	for (auto& DiscardEvent : Context.DiscardEventMap)
	{
		DiscardEvent.Value.ExecuteIfBound();
	}

}

bool UDialogueState_Exit::CanTransition(FDialogueTaskContext& Context)
{
	Reset();
	return true;
}