// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NativeGameplayTags.h"
#include "GameMapManagerSubsystem.generated.h"

DECLARE_DELEGATE_ThreeParams(FOnSmallMapChangedDelegate, UTexture2D*, FVector2D, float);

UCLASS(BlueprintType)
class GAMEUI_API UMapManager_TravelRequst : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = MapManager, meta = (AllowedTypes = "Map"))
	FPrimaryAssetId MapID;

	UPROPERTY(BlueprintReadWrite, Category = MapManager)
	TMap<FString, FString> ExtraArgs;
public:
	/** Returns the full map name that will be used during gameplay */
	virtual FString GetMapName() const;

	/** Constructs the full URL that will be passed to ServerTravel */
	virtual FString ConstructTravelURL() const;
};

USTRUCT(BlueprintType)
struct FSmallMapInfo
{
	GENERATED_BODY()
public:
	FSmallMapInfo();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag SmallMapTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SmallMapName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Comment;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* SmallMapIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D RangeSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Scale = 1;
};

UCLASS(MinimalAPI)
class USmallMapDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = SmallMap)
	TArray<FSmallMapInfo> SmallMapInfoItems;

};

/**
 * 
 */
UCLASS(Config = Game)
class GAMEUI_API UGameMapManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	using FSmallMapChangedHandler = FOnSmallMapChangedDelegate;

public:
	UFUNCTION(BlueprintCallable)
	void SeamlessTravel(UMapManager_TravelRequst* Requst);

	/**
	 * @TODO:
	 *	Support asynchronous loading
	 */
	UFUNCTION(BlueprintCallable)
	void LoadSmallMapAsset(TSoftObjectPtr<USmallMapDefinition> SmallMapAsset);

	UFUNCTION(BlueprintCallable)
	FSmallMapInfo GetSmallMapInfo(FGameplayTag SmallMapTag);

	UFUNCTION(BlueprintCallable)
	void ResetSmallMapWithInfo(const FSmallMapInfo& info);

	UFUNCTION(BlueprintCallable)
	void ResetSmallMapWithTag(FGameplayTag SmallMapTag);

	FSmallMapChangedHandler& GetHandler_OnSmallMapChanged() {
		return SmallMapChangedHandler;
	};

private:
	UPROPERTY()
	TObjectPtr<USmallMapDefinition> SmallMapInfo;

	FSmallMapChangedHandler SmallMapChangedHandler;
};
