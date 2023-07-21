
#pragma once

#include "Engine/DataTable.h"
#include "CommonGame/DialogueAction_RuleAction.h"
#include "CommonGame/DialogueAction_SelectAction.h"
#include "GameplayTagContainer.h"
#include "DialogueTableRaw.generated.h"

enum class EDialogueSourceMode : uint8;
enum class EDialogueSelectionMode : uint8;

USTRUCT(BlueprintType)
struct FDialogOptionItem
{
	GENERATED_BODY()
public:
	// The Rule result will be used to determine whether the dialog option is available.
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDialogueAction_RuleAction> RlueAction;
	UPROPERTY(EditAnywhere)
	EDialogueSelectionMode SelectionMode;
	UPROPERTY(EditAnywhere)
	FText DialogueText;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UDataTable> DialogueTable;
	UPROPERTY(EditAnywhere, Category = "ExtraArgs", meta = (EditCondition = "SelectionMode == EDialogueSelectionMode::Primary"))
	TMap<FName, float> ExtraFloat;
	UPROPERTY(EditAnywhere, Category = "ExtraArgs", meta = (EditCondition = "SelectionMode == EDialogueSelectionMode::Primary"))
	TMap<FName, int32> ExtraInt;
	UPROPERTY(EditAnywhere, Category = "ExtraArgs", meta = (EditCondition = "SelectionMode == EDialogueSelectionMode::Primary"))
	TMap<FName, FString> ExtraString;
};

USTRUCT(BlueprintType)
struct COMMONSTORYRUNTIME_API FCommonDialogueTableRawBase : public FTableRowBase
{
	GENERATED_BODY()
public:
	FCommonDialogueTableRawBase();

	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems) {}
	
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) {}

public:
	UPROPERTY(EditAnywhere, Category = "Dialogue")
	EDialogueSourceMode SourceMode;

	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (EditCondition = "SourceMode == EDialogueSourceMode::NPC", Categories = "Gameplay.Role"))
	FGameplayTag IdentityTag;

	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (EditCondition = "SourceMode == EDialogueSourceMode::Player"))
	bool bEnableSelection;

	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (EditCondition = "!bEnableSelection"))
	FText DialogueText;

	UPROPERTY(EditAnywhere, Category = "NPC", meta = (EditCondition = "SourceMode == EDialogueSourceMode::NPC"))
	float TimeWaitForSkip;

	UPROPERTY(EditAnywhere, Category = "Dialogue Option", meta = (EditCondition = "SourceMode == EDialogueSourceMode::Player && bEnableSelection"))
	TArray<FDialogOptionItem> DialogOption;

	/**
	 * @TODO
	 * Currently a soft pointer, the resource is not loaded until it is used. 
	 * Consider bringing the loading time forward.
	 */
	UPROPERTY(EditAnywhere, Category = "Dialogue Option", meta = (EditCondition = "SourceMode == EDialogueSourceMode::Player && bEnableSelection"))
	TSubclassOf<UDialogueAction_SelectAction> SelectAction;
};