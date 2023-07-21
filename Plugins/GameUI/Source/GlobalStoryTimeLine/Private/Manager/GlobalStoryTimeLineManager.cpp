#include "Manager/GlobalStoryTimeLineManager.h"

#include "DataAsset/StoryTimeLineDataAsset.h"

UGlobalStoryTimeLineManager::UGlobalStoryTimeLineManager()
{
	GlobalTime = 0;
	GameTimeScale = 1;
}

void UGlobalStoryTimeLineManager::Initialize(FSubsystemCollectionBase& Collection)
{
	if (!StoryTimeLineDataAsset.IsNull())
	{
		if (const UStoryTimeLineDataAsset* data = StoryTimeLineDataAsset.LoadSynchronous())
		{
			for (const FStoryTimeNode& node : data->StoryTimeNode)
			{
				int32 GameTime = 0;
				switch (node.TimeUnit)
				{
				case ETimeUnitMode::Days:
					GameTime = node.Time * 86400;
					break;
				case ETimeUnitMode::Hours:
					GameTime = node.Time * 3600;
					break;
				case ETimeUnitMode::Minutes:
					GameTime = node.Time * 60;
					break;
				case ETimeUnitMode::Seconds:
					GameTime = node.Time;
					break;
				}

				TimeNodeQueue.Add(FTimeNode(GameTime, node.TimeNodeTag));
			}
			TimeNodeQueue.Sort([](const FTimeNode& a, const FTimeNode& b)
				{
					return a.GameTime > b.GameTime;
				}
			);
		}
	}
}

void UGlobalStoryTimeLineManager::Deinitialize()
{
	GetGameInstance()->GetTimerManager().ClearTimer(TimerHandle);
}

void UGlobalStoryTimeLineManager::RegisterTimeListener(FGameplayTag TimeNodeTag, FOnArrivalTime CallBack)
{
	CallBackHandleBucket& Bucket = CallBackHandleMap.FindOrAdd(TimeNodeTag);
	Bucket.Container.Add(CallBack);
}

bool UGlobalStoryTimeLineManager::ActiveTimeLine(float GameTimeScaleIn)
{
	GameTimeScale = GameTimeScaleIn;
	if (!TimerHandle.IsValid())
	{
		GetGameInstance()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::UpdateTimeLine, 1, true, -1);
	}
	return true;
}

void UGlobalStoryTimeLineManager::UpdateTimeLine()
{
	GlobalTime++;
	if (TimeNodeQueue.Num() > 0 && TimeNodeQueue.Top().GameTime * GameTimeScale <= GlobalTime)
	{
		if (CallBackHandleBucket* HandleBucket = CallBackHandleMap.Find(TimeNodeQueue.Top().TimeNodeTag))
		{
			for (FOnArrivalTime& Handle : HandleBucket->Container)
			{
				Handle.ExecuteIfBound();
			}
		}
		// When the execution is complete, the listening time node is removed
		TimeNodeQueue.RemoveAt(TimeNodeQueue.Num() - 1);
	}
}