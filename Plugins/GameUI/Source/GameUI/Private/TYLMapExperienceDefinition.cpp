// Copyright Epic Games, Inc. All Rights Reserved.

#include "TYLMapExperienceDefinition.h"

#include "GameMapManagerSubsystem.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(TYLMapExperienceDefinition)

UMapManager_TravelRequst* UTYLMapExperienceDefinition::CreateMapTravelRequest() const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();
	UMapManager_TravelRequst* Result = NewObject<UMapManager_TravelRequst>();
	Result->MapID = MapID;
	Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);

	return Result;
}

