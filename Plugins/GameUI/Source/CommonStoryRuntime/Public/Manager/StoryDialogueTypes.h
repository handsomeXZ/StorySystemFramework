#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework\Actor.h"
#include "StoryDialogueTypes.generated.h"

struct FStoryDialogueContext;
class UPlayerMappableInputConfig;

DECLARE_DELEGATE_OneParam(FOnReceiveHandler, FStoryDialogueContext&);
DECLARE_DELEGATE(FOnDiscardHandler);


UENUM()
enum class ESDTNodeType : uint8
{
	UnKnown,
	Entry,
	Content,
	Options,
	Selector,
	Action,
	Return,
	Continue,
	Exit,
};

UENUM()
enum class ESDTCommonActionType : uint8
{
	Generic,
	Blocking,
};

USTRUCT(BlueprintType)
struct FIndexHandle
{
	GENERATED_BODY()
public:
	bool operator==(const FIndexHandle& B) const
	{
		return NodeType == B.NodeType && Index == B.Index;
	}
	friend uint32 GetTypeHash(const FIndexHandle& B)
	{
		uint32 Hash = GetTypeHash(B.NodeType);
		return HashCombineFast(Hash, GetTypeHash(B.Index));
	}
	UPROPERTY()
	ESDTNodeType NodeType;
	UPROPERTY()
	int32 Index;
};

UENUM(BlueprintType)
enum class EDialogueSourceMode : uint8
{
	/** This dialogue is from an NPC */
	NPC,
	/** This dialogue is from an Player */
	Player,
};

UENUM(BlueprintType)
enum class EDialogueSelectionMode : uint8
{
	/** Primary dialog option, which can perform dialog option Action. And will end this dialog Select. */
	Primary,
	/** Secondary dialog option that only modifies the context of this dialog Select. */
	Secondary,
};

UENUM(BlueprintType)
enum class EDialogueType : uint8
{
	Single,
	Multiple,
};

UENUM(BlueprintType)
enum class EDialogueStateName : uint8
{
	UnKnown,
	InActive,
	Active,
	Execute,
	WaitingSingleDialogue,
	WaitingOptionsDialogue,
	WaitingBlockingAction,
	Exit,
};

struct FInputHandle
{
	FInputHandle()
	{
		InputBindingHandle = 0;
		InputConfig = nullptr;
	}
	uint32 InputBindingHandle;
	TObjectPtr<UPlayerMappableInputConfig> InputConfig;
};

USTRUCT(BlueprintType)
struct FDialogueItem
{
	GENERATED_BODY()
public:
	FDialogueItem() {}
	FDialogueItem(int32 IndexIn, FText& TextIn, bool bSelectedIn = false) : Index(IndexIn), Text(TextIn), bSelected(bSelectedIn) {}
	FDialogueItem(int32 IndexIn, FText& TextIn, TMap<FName, float> ExtraFloatIn, bool bSelectedIn = false) : Index(IndexIn), Text(TextIn), bSelected(bSelectedIn)
	{
		ExtraFloat.Empty();
		ExtraFloat.Append(ExtraFloatIn);
	}

	UPROPERTY(BlueprintReadOnly)
	int32 Index;
	UPROPERTY(BlueprintReadOnly)
	FText Text;
	UPROPERTY(BlueprintReadOnly)
	bool bSelected;
	UPROPERTY(BlueprintReadOnly)
	TMap<FName, float> ExtraFloat;
};

USTRUCT(BlueprintType)
struct FStoryDialogueContext
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	EDialogueType DialogueType;
	UPROPERTY(BlueprintReadOnly)
	TArray<FDialogueItem> Contents;
};

USTRUCT(BlueprintType)
struct FDialogueGlobalContext
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TMap<FName, float> FloatData;
	UPROPERTY(BlueprintReadWrite)
	TMap<FGameplayTag, TObjectPtr<AActor>> Targets;
	UPROPERTY()
	TObjectPtr<UActionToContinue> ActionToContinue;
};

UCLASS()
class UActionToContinue : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void ContinueExc() { bContinue = true; }

	bool CanContinue() { return bContinue; }

private:
	bool bContinue = false;
};