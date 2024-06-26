// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Engine/Blueprint.h"
#include "GameplayTagContainer.h"
#include "Templates/TypeHash.h"

#include "Manager/StoryDialogueTypes.h"

#include "StoryDialogueTree.generated.h"


class UDialogueAction_RuleAction;
class UInputAction;
class UPlayerMappableInputConfig;

USTRUCT()
struct FSDTDContentNode
{
	GENERATED_BODY()

	UPROPERTY()
	FIndexHandle NodeIndex;
	UPROPERTY()
	FIndexHandle Child;
	UPROPERTY()
	EDialogueSourceMode SourceMode = EDialogueSourceMode::NPC;
	UPROPERTY()
	FGameplayTag IdentityTag;
	UPROPERTY()
	FText Content;
	UPROPERTY()
	float TimeWaitForSkip = -1;
};

USTRUCT()
struct FSDTDOptionItem
{
	GENERATED_BODY()

	UPROPERTY()
	FText Content;
	UPROPERTY()
	bool bSelected = false;
	UPROPERTY()
	TObjectPtr<UDialogueAction_RuleAction> RuleAction;
	UPROPERTY()
	TMap<FName, float> ExtraFloat;
};

USTRUCT()
struct FSDTDOptionContainer
{
	GENERATED_BODY()
	UPROPERTY()
	FIndexHandle IndexHandle;
	UPROPERTY()
	FSDTDOptionItem OptionItem;
};

USTRUCT()
struct FSDTDOptionsNode
{
	GENERATED_BODY()
	UPROPERTY()
	FIndexHandle NodeIndex;
	UPROPERTY()
	TArray<FIndexHandle> Children;
	UPROPERTY()
	TArray<FSDTDOptionContainer> Options;
};

USTRUCT()
struct FSDTDSelectorNode
{
	GENERATED_BODY()
	UPROPERTY()
	FIndexHandle NodeIndex;
	UPROPERTY()
	FIndexHandle TrueIndex;
	UPROPERTY()
	FIndexHandle FalseIndex;
	UPROPERTY()
	TObjectPtr<USDTDSelectorAction> SelectorInstance;
	UPROPERTY()
	TSubclassOf<USDTDSelectorAction> SelectorClass;
};

USTRUCT()
struct FSDTDActionNode
{
	GENERATED_BODY()
	UPROPERTY()
	FIndexHandle NodeIndex;
	UPROPERTY()
	FIndexHandle Child;
	UPROPERTY()
	ESDTCommonActionType ActionType = ESDTCommonActionType::Generic;
	UPROPERTY()
	TObjectPtr<USDTDCommonAction> CommonActionInstance;
	UPROPERTY()
	TSubclassOf<USDTDCommonAction> CommonActionClass;
};

USTRUCT()
struct FSDTDReturnNode
{
	GENERATED_BODY()
	UPROPERTY()
	FIndexHandle NodeIndex;
	UPROPERTY()
	FIndexHandle Child;
};

UCLASS(Abstract)
class COMMONSTORYRUNTIME_API USDTDActionBase : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Attention:
	 * 1. The SelectedBit Info is saved as uint8, which supports up to 128 options.
	 * 2. SelectedBit saves only the Prev selection, and Current selection is not recorded.
	 * 3. Index Start from 0.
	 */
	UFUNCTION(BlueprintCallable)
	bool CheckSelectedByBitInfo(int32 Index, uint8 SelectedBitInfo) const;

};

UCLASS(EditInlineNew, Blueprintable, BlueprintType, Abstract)
class COMMONSTORYRUNTIME_API USDTDSelectorAction : public USDTDActionBase
{
	GENERATED_BODY()
public:
	virtual class UWorld* GetWorld() const override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Execute"))
	FDialogueGlobalContext Execute_BlueprintImplement(FDialogueGlobalContext Context, UWorld* World, bool& Result) const;

	virtual bool Execute(FDialogueGlobalContext& Context, FWorldContext& WorldContext) const;

};

UCLASS(EditInlineNew, Blueprintable, BlueprintType, Abstract)
class COMMONSTORYRUNTIME_API USDTDCommonAction : public USDTDActionBase
{
	GENERATED_BODY()
public:
	virtual class UWorld* GetWorld() const override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Execute"))
	FDialogueGlobalContext Execute_BlueprintImplement(FDialogueGlobalContext Context, UWorld* World) const;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Execute And Blocking"))
	void ExecuteAndBlocking_BlueprintImplement(FDialogueGlobalContext Context, UWorld* World) const;

	UFUNCTION(BlueprintCallable, meta=(HideSelfPin))
	virtual void ContinueExc(FDialogueGlobalContext Context) const;

	virtual void Execute(FDialogueGlobalContext& Context, FWorldContext& WorldContext) const;

};

UCLASS(BlueprintType)
class COMMONSTORYRUNTIME_API UStoryDialogueTree : public UObject
{
	GENERATED_UCLASS_BODY()

	/** root index of loaded tree */
	UPROPERTY()
	FIndexHandle RootIndex;

	UPROPERTY()
	TArray<FSDTDContentNode> ContentNodeList;
	
	UPROPERTY()
	TArray<FSDTDOptionsNode> OptionsNodeList;

	UPROPERTY()
	TArray<FSDTDSelectorNode> SelectorNodeList;

	UPROPERTY()
	TArray<FSDTDActionNode> ActionNodeList;

	UPROPERTY()
	TArray<FSDTDReturnNode> ReturnNodeList;

	// Used for bReliable
	UPROPERTY()
	TArray<FGameplayTag> IdentifyTagList;

	// Check the IdentifyTags, execute the conversation only if all the IdentifyTags are present
	UPROPERTY(EditAnywhere, Category = DefaultEditing)
	bool bReliable;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	TObjectPtr<UPlayerMappableInputConfig> InputConfig;

#if WITH_EDITORONLY_DATA

	/** Graph for Behavior Tree */
	UPROPERTY()
	TObjectPtr<class UEdGraph>	EdGraph;

	/** Info about the graphs we last edited */
/*	UPROPERTY()
	TArray<FEditedDocumentInfo> LastEditedDocuments;*/

#endif

	int32 GetNodeListNum(ESDTNodeType NodeType)
	{
		switch (NodeType)
		{
		case ESDTNodeType::Content:
			return ContentNodeList.Num();
		case ESDTNodeType::Options:
			return OptionsNodeList.Num();
		case ESDTNodeType::Selector:
			return SelectorNodeList.Num();
		case ESDTNodeType::Action:
			return ActionNodeList.Num();
		case ESDTNodeType::Return:
			return ReturnNodeList.Num();
		case ESDTNodeType::Continue:
			return ReturnNodeList.Num();
		default:
			return 0;
		}
	}

	void Empty()
	{
		ContentNodeList.Empty();
		OptionsNodeList.Empty();
		SelectorNodeList.Empty();
		ActionNodeList.Empty();
		ReturnNodeList.Empty();
		IdentifyTagList.Empty();
	}

};
