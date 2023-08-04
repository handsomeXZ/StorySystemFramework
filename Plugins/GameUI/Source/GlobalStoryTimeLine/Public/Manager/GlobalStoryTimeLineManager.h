#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "DataAsset/StoryTimeLineDataAsset.h"

#include "GlobalStoryTimeLineManager.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnArrivalTime);

UCLASS(Config = Game)
class GLOBALSTORYTIMELINE_API UGlobalStoryTimeLineManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UGlobalStoryTimeLineManager();

	// USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize();
	// ~ End USubsystem

	UFUNCTION(BlueprintCallable)
	void RegisterTimeListener(FGameplayTag TimeNodeTag, FOnArrivalTime CallBack);

	/**
	 * GameTimeScale = Reality time difference / Game time difference
	 */
	UFUNCTION(BlueprintCallable)
	bool ActiveTimeLine(float GameTimeScale);

	UFUNCTION(BlueprintCallable)
	void TimeFastForward(float TimeDelta);
	UFUNCTION(BlueprintCallable)
	void TimeFastForwardToNextTimeNode();
	UFUNCTION(BlueprintCallable)
	void SetGlobalTimeDilation(float TimeDilation) { GlobalTimeDilation = TimeDilation; }

	void UpdateTimeLine();

private:
	struct CallBackHandleBucket
	{
		TArray<FOnArrivalTime> Container;
	};
	struct FTimeNode
	{
		FTimeNode() {}
		FTimeNode(int32 GameTimeIn, FGameplayTag TimeNodeTagIn)
			: GameTime(GameTimeIn), TimeNodeTag(TimeNodeTagIn) {}

		int32 GameTime;
		FGameplayTag TimeNodeTag;
	};
	// Reality time difference
	int32 GlobalTime;
	// GameTimeScale = Reality time difference / Game time difference
	float GameTimeScale;
	float GlobalTimeDilation;
	FTimerHandle TimerHandle;
	TArray<FTimeNode> TimeNodeQueue;
	TMap<FGameplayTag, CallBackHandleBucket> CallBackHandleMap;

	UPROPERTY(config, EditAnywhere)
	TSoftObjectPtr<UStoryTimeLineDataAsset> StoryTimeLineDataAsset;
};


