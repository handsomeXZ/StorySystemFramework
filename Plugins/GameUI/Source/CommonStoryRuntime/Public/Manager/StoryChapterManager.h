// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UObject/PrimaryAssetId.h"
#include "GameplayTagContainer.h"
#include "StoryChapterManager.generated.h"

class UStoryChapterTree;
class UStoryDialogueTree;
class USCTNode_Chapter;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStoryChapterChanged);


UCLASS(Config = Game)
class COMMONSTORYRUNTIME_API UStoryChapterManager : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
public:
	UStoryChapterManager();

	//~ Begin FTickableGameObject Interface
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual ETickableTickType GetTickableTickType() const override;
	//~ End FTickableGameObject Interface

	// USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize();
	// ~ End USubsystem

	void Update();

	UFUNCTION(BlueprintCallable)
	void ActiveStory();

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetCurrentStoryChapterTag();

	UFUNCTION(BlueprintCallable)
	FString GetCurrentStoryChapterName();

	UFUNCTION(BlueprintCallable)
	UStoryDialogueTree* GetStoryDialogueTree(FGameplayTag IdentifyTag, bool bReadAdded = false);


public:
	UPROPERTY(BlueprintAssignable, Category = "MulDelegate")
	FOnStoryChapterChanged OnStoryChapterChanged;

private:
	UPROPERTY(EditAnywhere)
	FGameplayTag CurrentStoryChapterTag;
	
	UPROPERTY(EditAnywhere)
	FGameplayTag PrevStoryChapterTag;

	UPROPERTY()
	TObjectPtr<UStoryChapterTree> StoryChapterTree;
	UPROPERTY()
	TObjectPtr<USCTNode_Chapter> CurrentChapter;
};