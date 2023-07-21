
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health_ActorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealth_DeathEvent, AActor*, OwningActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealth_AttributeChanged, float, OldValue, float, NewValue);

UCLASS(Blueprintable, Meta = (DisplayName = "Health Actor Component", BlueprintSpawnableComponent))
class GAMEUI_API UHealth_ActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealth_ActorComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "Health")
	static UHealth_ActorComponent* FindHealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UHealth_ActorComponent>() : nullptr); }

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetDefaultHealth(float value, bool bUpdateCurrentValue)
	{
		DefaultHealth = value; 
		CurrentHealth = bUpdateCurrentValue? DefaultHealth:CurrentHealth;
	}

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Damage(float value);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float value);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth() { return CurrentHealth; };

public:

	// Delegate fired when the health value has changed.
	UPROPERTY(BlueprintAssignable)
	FHealth_DeathEvent OnDeath;

	// Delegate fired when the max health value has changed.
	UPROPERTY(BlueprintAssignable)
	FHealth_AttributeChanged OnHealthChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	float DefaultHealth;

	UPROPERTY()
	float CurrentHealth;
};