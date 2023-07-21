
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/PrimaryAssetId.h"
#include "ProofsManager.generated.h"

USTRUCT(BlueprintType)
struct FCommonProofItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ProofItemTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ProofItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Comment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

};


UCLASS(MinimalAPI)
class UCommonProofDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Proofs)
	TArray<FCommonProofItem> ProofItems;

};


UCLASS(Config = Game)
class GAMEUI_API UProofsManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UProofsManager();

	// USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize();
	// ~ End USubsystem

	UFUNCTION(BlueprintCallable)
	TArray<FCommonProofItem> GetCollectedProofs() { return CollectedProofs; }

	UFUNCTION(BlueprintCallable)
	bool CollectProof(FGameplayTag ProofItemTag);

private:
	UPROPERTY(Config, EditAnywhere)
	TSoftObjectPtr<UCommonProofDefinition> DefaultProofsAsset;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, FCommonProofItem> ProofsList;

	UPROPERTY(EditAnywhere)
	TArray<FCommonProofItem> CollectedProofs;

	UPROPERTY(EditAnywhere)
	TSet<FGameplayTag> CollectedProofTags;
};