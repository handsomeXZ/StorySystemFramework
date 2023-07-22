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
		SwitchState(PrevNodeType, CurrentNodeType);

		// Execute immediately after the state Switch
		Execute(Context);
	}
}

void UDialogueTaskStateMachine::SwitchState(ESDTNodeType PrevNodeTypeIn, ESDTNodeType NextNodeTypeIn)
{
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

//////////////////////////////////////////////////////////////////////////
// UDialogueState_InActive

bool UDialogueState_InActive::CanTransition(FDialogueTaskContext& Context)
{
	if (Context.StoryDialogueTree != nullptr && Context.PlayerOnReceiveDelegate.IsBound() && Context.PlayerOnDiscardDelegate.IsBound())
	{
		if (Context.StoryDialogueTree->bReliable)
		{
			for (FGameplayTag tag : Context.StoryDialogueTree->IdentifyTagList)
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
// UDialogueState_Execute

void UDialogueState_Execute::Execute(FDialogueTaskContext& Context)
{
	FIndexHandle IndexHandle = Context.CurrentIndexHandle;
	switch (IndexHandle.NodeType)
	{
	case ESDTNodeType::Content:
	{
		FSDTDContentNode Node = Context.StoryDialogueTree->ContentNodeList[IndexHandle.Index];
		if (Node.SourceMode == EDialogueSourceMode::Player)
		{
			FStoryDialogueContext MyContext;
			MyContext.DialogueType = EDialogueType::Single;
			MyContext.Contents.Add(FDialogueItem(Node.Child, Node.Content));

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
				

				FStoryDialogueContext MyContext;
				MyContext.DialogueType = EDialogueType::Single;
				MyContext.Contents.Add(FDialogueItem(Node.Child, Node.Content));

				handle->ExecuteIfBound(MyContext);
			}
		}

		break;
	}
	case ESDTNodeType::Options:
	{
		FSDTDOptionsNode Node = Context.StoryDialogueTree->OptionsNodeList[IndexHandle.Index];
		FStoryDialogueContext MyContext;
		MyContext.DialogueType = EDialogueType::Multiple;

		for (auto& Elem : Node.Options)
		{
			bool RlueResult = true;
			if (Elem.Value.RuleAction)
			{
				if (UDialogueAction_RuleAction* Action = Elem.Value.RuleAction)
				{
					Action->Initialize(Context.WorldContext);
					Action->ExecuteAction(MyContext, RlueResult);
				}
				
			}

			if (RlueResult)
			{
				MyContext.Contents.Add(FDialogueItem(Elem.Key, Elem.Value.Content, Elem.Value.bSelected));
			}
			
			
		}

		Context.PlayerOnReceiveDelegate.ExecuteIfBound(MyContext);

		break;
	}
	case ESDTNodeType::Selector:
	{
		FSDTDSelectorNode Node = Context.StoryDialogueTree->SelectorNodeList[IndexHandle.Index];
		if (Node.SelectorInstance)
		{
			ResultOfSelector = Node.SelectorInstance->Execute();
		}
		else if (Node.SelectorClass)
		{
			// CDO
			const USDTDSelectorAction* Action = GetDefault<USDTDSelectorAction>(Node.SelectorClass);
			ResultOfSelector = Action->Execute();
		}
		break;
	}
	case ESDTNodeType::Action:
	{
		FSDTDActionNode Node = Context.StoryDialogueTree->ActionNodeList[IndexHandle.Index];

		if (Node.CommonActionInstance)
		{
			Node.CommonActionInstance->Execute();
		}
		else if (Node.CommonActionClass)
		{
			// CDO
			const USDTDCommonAction* Action = GetDefault<USDTDCommonAction>(Node.CommonActionClass);
			Action->Execute();
		}

		break;
	}
	}
}

bool UDialogueState_Execute::CanTransition(FDialogueTaskContext& Context)
{
	FIndexHandle IndexHandle = Context.CurrentIndexHandle;
	switch (IndexHandle.NodeType)
	{
	case ESDTNodeType::Action:
	{
		FSDTDActionNode Node = Context.StoryDialogueTree->ActionNodeList[IndexHandle.Index];
		Context.CurrentIndexHandle = Node.Child;

		break;
	}
	case ESDTNodeType::Selector:
	{
		FSDTDSelectorNode Node = Context.StoryDialogueTree->SelectorNodeList[IndexHandle.Index];
		Context.CurrentIndexHandle = ResultOfSelector ? Node.TrueIndex : Node.FalseIndex;

		break;
	}
	case ESDTNodeType::Continue:
	{
		FSDTDReturnNode Node = Context.StoryDialogueTree->ReturnNodeList[IndexHandle.Index];
		Context.CurrentIndexHandle = Node.Child;

		break;
	}
	case ESDTNodeType::Return:
	{
		FSDTDReturnNode Node = Context.StoryDialogueTree->ReturnNodeList[IndexHandle.Index];
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
	bIsActive = (Context.StoryDialogueTree->InputAction) ? false : true;

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
		Context.TimeWaitForSkip -= Context.DeltaSeconds / 1000.0f;
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
		FIndexHandle IndexHandle = Context.CurrentIndexHandle;
		FSDTDContentNode Node = Context.StoryDialogueTree->ContentNodeList[IndexHandle.Index];

		if (Node.SourceMode == EDialogueSourceMode::Player)
		{
			Context.PlayerOnDiscardDelegate.Execute();
		}
		else if (const FOnDiscardHandler* handle = Context.DiscardEventMap.Find(Node.IdentityTag))
		{
			handle->Execute();
		}

		Context.CurrentIndexHandle = Node.Child;
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

void UDialogueState_WaitingOptionsDialogue::ActiveThisState(FIndexHandle OptionID)
{
	SelectedOptionID = OptionID;
	bIsActive = true;
}

bool UDialogueState_WaitingOptionsDialogue::CanTransition(FDialogueTaskContext& Context)
{
	if (bIsActive)
	{
		Context.SelectOptionDelegate.Unbind();
		Context.PlayerOnDiscardDelegate.Execute();

		Context.CurrentIndexHandle = SelectedOptionID;

		Reset();
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// UDialogueState_Exit

void UDialogueState_Exit::Execute(FDialogueTaskContext& Context)
{
	Context.bTimeWaitForSkip = false;
	Context.TimeWaitForSkip = 0;
	Context.CurrentIndexHandle.Index = -1;
	Context.CurrentIndexHandle.NodeType = ESDTNodeType::UnKnown;

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
	Context.StoryDialogueTree = nullptr;
}

bool UDialogueState_Exit::CanTransition(FDialogueTaskContext& Context)
{
	Reset();
	return true;
}