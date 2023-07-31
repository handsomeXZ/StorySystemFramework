// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Manager/StoryDialogueTypes.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "Manager/DialogueStateMachine.h"
#include "DialogueManager.generated.h"

struct FCommonDialogueTableRawBase;

class UDialogueAssetDefinition;
class UDataTable;
class UEnhancedInputComponent;
class UPlayerMappableInputConfig;
class UInputAction;
class UCommonActivatableWidget;


/**
 * 
 */
UCLASS()
class COMMONSTORYRUNTIME_API UDialogueManager : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
public:
	UDialogueManager();
	//~ Begin FTickableGameObject Interface
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual ETickableTickType GetTickableTickType() const override;
	//~ End FTickableGameObject Interface

	FOnReceiveHandler& OnPlayerReceiveDialogue() { return Context.PlayerOnReceiveDelegate; }

	bool ActivateDialogueTask(FGameplayTag IdentityTag);
	bool RegisterForDialogueTask(FGameplayTag IdentityTag, AActor* Owner, FOnReceiveHandler ReceiveHandler, FOnDiscardHandler OnDiscardHandler);
	void UnRegisterForDialogueTask(FGameplayTag IdentityTag);
	void RegisterPlayer(FOnReceiveHandler PlayerOnReceiveDelegate, FOnDiscardHandler PlayerOnDiscardDelegate);
	void UnRegisterPlayer();

	UFUNCTION(BlueprintCallable)
	AActor* FindOwnerByIdentity(FGameplayTag IdentityTag);

	UFUNCTION(BlueprintCallable)
	void SelectDialogue(int32 OptionID);
	UFUNCTION(BlueprintCallable)
	void ContinueContentDialogue();
	UFUNCTION(BlueprintCallable)
	void CancelDialogueTask();

	void UnBindInput();

	void ResetManager();

private:
	UPROPERTY()
	TObjectPtr<UDialogueTaskStateMachine> StateMachine;
	UPROPERTY()
	FDialogueTaskContext Context;
};
