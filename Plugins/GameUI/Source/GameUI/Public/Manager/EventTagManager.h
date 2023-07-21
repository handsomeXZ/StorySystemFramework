
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "EventTagManager.generated.h"


UCLASS(Config = Game)
class GAMEUI_API UEventTagManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UEventTagManager();

	// USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize();
	// ~ End USubsystem

	UFUNCTION(BlueprintCallable)
	void UpdateTag(const FGameplayTag& Tag);
	UFUNCTION(BlueprintCallable)
	void UpdateTags(const FGameplayTagContainer& Container);
	UFUNCTION(BlueprintCallable)
	bool HasEventTag(const FGameplayTag& Tag, bool bIsExact = true);
	UFUNCTION(BlueprintCallable)
	bool HasEventTags(const FGameplayTagContainer& Container, bool bIsExact = true, bool bIsAllMatching = true);

private:
	FGameplayTagContainer EventTagContainer;
};