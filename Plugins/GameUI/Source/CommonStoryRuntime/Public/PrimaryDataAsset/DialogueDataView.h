
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogueDataView.generated.h"


UCLASS(BlueprintType, Const)
class COMMONSTORYRUNTIME_API UDialogueDataView : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UDialogueDataView(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif
	//~End of UObject interface

	//~UPrimaryDataAsset interface
#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif
	//~End of UPrimaryDataAsset interface

public:
	/**
	 * Gameplay tag for each episode
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Dialogue", meta = (Categories = "Gameplay.StoryChapter"))
	FGameplayTag EpisodeTag;

	/**
	 * If true, Allows the Dialogue component to Skip by Input
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
	bool bEnableInput;


	/**
	 * 
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
	TSoftObjectPtr<UDataTable> DialogueTable;


	UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
	TSoftObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
	TSoftObjectPtr<UPlayerMappableInputConfig> InputConfig;

};
