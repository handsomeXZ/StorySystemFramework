// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMapManagerSubsystem.h"
#include "ExperienceDefinition.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_GAMEPLAY_SMALLMAP_DEFAULT, "Gameplay.SmallMap.Default");

FSmallMapInfo::FSmallMapInfo()
{
	SmallMapTag = TAG_GAMEPLAY_SMALLMAP_DEFAULT;
}

FString UMapManager_TravelRequst::GetMapName() const
{
	FAssetData MapAssetData;
	if (UAssetManager::Get().GetPrimaryAssetData(MapID, /*out*/ MapAssetData))
	{
		return MapAssetData.PackageName.ToString();
	}
	else
	{
		return FString();
	}
}

FString UMapManager_TravelRequst::ConstructTravelURL() const
{
	FString CombinedExtraArgs;
	for (const auto& KVP : ExtraArgs)
	{
		if (!KVP.Key.IsEmpty())
		{
			if (KVP.Value.IsEmpty())
			{
				CombinedExtraArgs += FString::Printf(TEXT("?%s"), *KVP.Key);
			}
			else
			{
				CombinedExtraArgs += FString::Printf(TEXT("?%s=%s"), *KVP.Key, *KVP.Value);
			}
		}
	}
	return FString::Printf(TEXT("%s%s"),
		*GetMapName(),
		*CombinedExtraArgs);
}


void UGameMapManagerSubsystem::SeamlessTravel(UMapManager_TravelRequst* Requst)
{
	GetWorld()->ServerTravel(Requst->ConstructTravelURL());
}

void UGameMapManagerSubsystem::LoadSmallMapAsset(TSoftObjectPtr<USmallMapDefinition> SmallMapAsset)
{
	SmallMapInfo = SmallMapAsset.LoadSynchronous();
}

FSmallMapInfo UGameMapManagerSubsystem::GetSmallMapInfo(FGameplayTag SmallMapTag)
{
	if (SmallMapInfo.Get() != nullptr)
	{
		for (const FSmallMapInfo& info : SmallMapInfo->SmallMapInfoItems)
		{
			if (info.SmallMapTag == SmallMapTag)
			{
				return info;
			}
		}
	}

	return FSmallMapInfo();
}

void UGameMapManagerSubsystem::ResetSmallMapWithInfo(const FSmallMapInfo& info)
{
	SmallMapChangedHandler.ExecuteIfBound(info.SmallMapIcon, info.RangeSize, info.Scale);
}

void UGameMapManagerSubsystem::ResetSmallMapWithTag(FGameplayTag SmallMapTag)
{
	if (SmallMapInfo.Get() != nullptr)
	{
		for (const FSmallMapInfo& info : SmallMapInfo->SmallMapInfoItems)
		{
			if (info.SmallMapTag == SmallMapTag)
			{
				ResetSmallMapWithInfo(info);
				return;
			}
		}
	}
	
}