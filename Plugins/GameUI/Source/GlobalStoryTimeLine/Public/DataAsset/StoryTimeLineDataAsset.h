
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StoryTimeLineDataAsset.generated.h"

UENUM(BlueprintType)
enum class ETimeUnitMode : uint8
{
	Days,
	Hours,
	Minutes,
	Seconds
};

USTRUCT(BlueprintType)
struct FStoryTimeNode
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere)
	FGameplayTag TimeNodeTag;
	UPROPERTY(EditAnywhere)
	ETimeUnitMode TimeUnit = ETimeUnitMode::Days;
	UPROPERTY(EditAnywhere)
	int32 Time = 0;
};

UCLASS(BlueprintType, Const)
class GLOBALSTORYTIMELINE_API UStoryTimeLineDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UStoryTimeLineDataAsset();

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
	UPROPERTY(EditDefaultsOnly, Category = "StoryTime", meta = (Categories = "Gameplay.StoryTimeNode"))
	TArray<FStoryTimeNode> StoryTimeNode;
};
