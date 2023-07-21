// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/PrimaryAssetId.h"

#include "TYLMapExperienceDefinition.generated.h"


class UMapManager_TravelRequst;
class UTexture2D;
class UUserWidget;

/** Description of settings used to display experiences in the UI and start a new session */
UCLASS(MinimalAPI)
class UTYLMapExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** The specific map to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience, meta=(AllowedTypes="Map"))
	FPrimaryAssetId MapID;

	/** The gameplay experience to load */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience, meta=(AllowedTypes="ExperienceDefinition"))
	FPrimaryAssetId ExperienceID;

public:
	/** Create a request object that is used to actually start a session with these settings */
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	UMapManager_TravelRequst* CreateMapTravelRequest() const;
};
