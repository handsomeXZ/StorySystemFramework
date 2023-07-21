#include "CommonGame/Health_ActorComponent.h"



UHealth_ActorComponent::UHealth_ActorComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void UHealth_ActorComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = DefaultHealth;
}

void UHealth_ActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UHealth_ActorComponent::Damage(float value)
{
	OnHealthChanged.Broadcast(CurrentHealth, CurrentHealth - value);
	CurrentHealth -= value;
	if (CurrentHealth <= 0)
	{
		AActor* Owner = GetOwner();
		check(Owner);
		OnDeath.Broadcast(Owner);
	}
}

void UHealth_ActorComponent::Heal(float value)
{
	OnHealthChanged.Broadcast(CurrentHealth, (CurrentHealth + value) > DefaultHealth ? DefaultHealth:(CurrentHealth + value));
	CurrentHealth += value;
	if (CurrentHealth > DefaultHealth)
	{
		CurrentHealth = DefaultHealth;
	}
}