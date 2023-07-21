// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "GameFeaturePluginOperationResult.h"
//#include "LoadingProcessInterface.h"

#include "ExperienceManagerComponent.generated.h"

class UExperienceDefinition;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnExperienceLoaded, const UExperienceDefinition* /*Experience*/);

enum class EExperienceLoadState
{
	Unloaded,
	Loading,
	LoadingGameFeatures,
	LoadingChaosTestingDelay,
	ExecutingActions,
	Loaded,
	Deactivating
};

UCLASS()
class UExperienceManagerComponent final : public UGameStateComponent/*, public ILoadingProcessInterface*/
{
	GENERATED_BODY()

public:

	UExperienceManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface

	////~ILoadingProcessInterface interface
	//virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;
	////~End of ILoadingProcessInterface


	void SetCurrentExperience(FPrimaryAssetId ExperienceId);


	// Ensures the delegate is called once the experience has been loaded,
	// before others are called.
	// However, if the experience has already loaded, calls the delegate immediately.
	void CallOrRegister_OnExperienceLoaded_HighPriority(FOnExperienceLoaded::FDelegate&& Delegate);

	// Ensures the delegate is called once the experience has been loaded
	// If the experience has already loaded, calls the delegate immediately
	void CallOrRegister_OnExperienceLoaded(FOnExperienceLoaded::FDelegate&& Delegate);

	// Ensures the delegate is called once the experience has been loaded
	// If the experience has already loaded, calls the delegate immediately
	void CallOrRegister_OnExperienceLoaded_LowPriority(FOnExperienceLoaded::FDelegate&& Delegate);

	// This returns the current experience if it is fully loaded, asserting otherwise
	// (i.e., if you called it too soon)
	const UExperienceDefinition* GetCurrentExperienceChecked() const;

	// Returns true if the experience is fully loaded
	bool IsExperienceLoaded() const;

private:
	UFUNCTION()
	void OnRep_CurrentExperience();

	void StartExperienceLoad();
	void OnExperienceLoadComplete();
	void OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result);
	void OnExperienceFullLoadCompleted();

	void OnActionDeactivationCompleted();
	void OnAllActionsDeactivated();

private:
	UPROPERTY(ReplicatedUsing=OnRep_CurrentExperience)
	TObjectPtr<const UExperienceDefinition> CurrentExperience;

	EExperienceLoadState LoadState = EExperienceLoadState::Unloaded;

	int32 NumGameFeaturePluginsLoading = 0;
	TArray<FString> GameFeaturePluginURLs;

	int32 NumObservedPausers = 0;
	int32 NumExpectedPausers = 0;

	/**
	 * Delegate called when the experience has finished loading just before others
	 * (e.g., subsystems that set up for regular gameplay)
	 */
	FOnExperienceLoaded OnExperienceLoaded_HighPriority;

	/** Delegate called when the experience has finished loading */
	FOnExperienceLoaded OnExperienceLoaded;

	/** Delegate called when the experience has finished loading */
	FOnExperienceLoaded OnExperienceLoaded_LowPriority;
};
