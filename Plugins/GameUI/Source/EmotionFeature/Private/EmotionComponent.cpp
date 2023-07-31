// Fill out your copyright notice in the Description page of Project Settings.
#include "EmotionComponent.h"

// Sets default values for this component's properties
UEmotionComponent::UEmotionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	AlertValue.EmotionType = EEmotionType::Alert;
	EcstaticValue.EmotionType = EEmotionType::Ecstatic;
	AdmirationValue.EmotionType = EEmotionType::Admiration;
	RageValue.EmotionType = EEmotionType::Rage;
	// ...
}


// Called when the game starts
void UEmotionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEmotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEmotionComponent::ApplyRuleModifier(UEmotionalRuleModifier* RuleModifierIn)
{
	bEnableRlueModifier = true;
	RuleModifier = RuleModifierIn;
}

TArray<FEmotionParameter> UEmotionComponent::GetEmotion()
{
	TArray<FEmotionParameter> EmotionParameters;
	EmotionParameters.Add(AlertValue);
	EmotionParameters.Add(EcstaticValue);
	EmotionParameters.Add(AdmirationValue);
	EmotionParameters.Add(RageValue);

	if (bEnableRlueModifier)
	{
		return RuleModifier->ExecuteModifier(EmotionParameters);
	}
	else
	{
		return EmotionParameters;
	}
}

void UEmotionComponent::ApplyEmotionEffect(EEmotionEffectType Type, FEmotionParameter EmotionDelta, float DecayRate)
{
	switch (Type)
	{
	case EEmotionEffectType::Single:
	{
		ApplyEffect_Single(EmotionDelta, 1.f);
		break;
	}
	case EEmotionEffectType::Related:
	{
		ApplyEffect_Related(EmotionDelta, DecayRate);
		break;
	}
	case EEmotionEffectType::InRange:
	{
		ApplyEffect_InRange(EmotionDelta, DecayRate);
		break;
	}
	}
}

void UEmotionComponent::UpdateEmotion(FEmotionParameter& Target, FEmotionParameter EmotionDelta, float DecayRate)
{
	if (RuleModifier)
	{
		RuleModifier->MarkDirty();
	}
	Target.IntensityValue += EmotionDelta.IntensityValue * DecayRate;
	Target.Priority += EmotionDelta.IntensityValue * DecayRate;
}

void UEmotionComponent::ApplyEffect_Single(FEmotionParameter EmotionDelta, float DecayRate)
{
	switch (EmotionDelta.EmotionType)
	{
	case EEmotionType::Alert: 
	{
		UpdateEmotion(AlertValue, EmotionDelta, DecayRate);
		break;
	}
	case EEmotionType::Ecstatic:
	{
		UpdateEmotion(EcstaticValue, EmotionDelta, DecayRate);
		break;
	}
	case EEmotionType::Admiration:
	{
		UpdateEmotion(AdmirationValue, EmotionDelta, DecayRate);
		break;
	}
	case EEmotionType::Rage:
	{
		UpdateEmotion(RageValue, EmotionDelta, DecayRate);
		break;
	}
	}
}

void UEmotionComponent::ApplyEffect_Related(FEmotionParameter EmotionDelta, float DecayRate)
{
	ApplyEffect_Single(EmotionDelta, 1);
	for (AActor* target : RelatedTarget)
	{
		if (UEmotionComponent* EmotionComp = target->FindComponentByClass<UEmotionComponent>())
		{
			EmotionComp->ApplyEffect_Single(EmotionDelta, DecayRate);
		}
	}
}

void UEmotionComponent::ApplyEffect_InRange(FEmotionParameter EmotionDelta, float DecayRate)
{
	// Empty...
}

//////////////////////////////////////////////////////////////////////////

void UEmotionComponent::TryApplyRuleModifier(UObject* Target, UEmotionalRuleModifier* RuleModifierIn)
{
	if (AActor* actor = Cast<AActor>(Target))
	{
		if (UEmotionComponent* EmotionComp = actor->FindComponentByClass<UEmotionComponent>())
		{
			EmotionComp->ApplyRuleModifier(RuleModifierIn);
		}
	}
	else if (UEmotionComponent* EmotionComp = Cast<UEmotionComponent>(Target))
	{
		EmotionComp->ApplyRuleModifier(RuleModifierIn);
	}
}

void UEmotionComponent::TryActivateRuleModifier(UObject* Target)
{
	if (AActor* actor = Cast<AActor>(Target))
	{
		if (UEmotionComponent* EmotionComp = actor->FindComponentByClass<UEmotionComponent>())
		{
			EmotionComp->ActivateRuleModifier();
		}
	}
	else if (UEmotionComponent* EmotionComp = Cast<UEmotionComponent>(Target))
	{
		EmotionComp->ActivateRuleModifier();
	}

}

void UEmotionComponent::TryInActivateRuleModifier(UObject* Target)
{
	if (AActor* actor = Cast<AActor>(Target))
	{
		if (UEmotionComponent* EmotionComp = actor->FindComponentByClass<UEmotionComponent>())
		{
			EmotionComp->InActivateRuleModifier();
		}
	}
	else if (UEmotionComponent* EmotionComp = Cast<UEmotionComponent>(Target))
	{
		EmotionComp->InActivateRuleModifier();
	}

}

void UEmotionComponent::TryApplyEmotionEffect(UObject* Target, EEmotionEffectType EffectType, FEmotionParameter EmotionDelta, float DecayRate)
{
	if (AActor* actor = Cast<AActor>(Target))
	{
		if (UEmotionComponent* EmotionComp = actor->FindComponentByClass<UEmotionComponent>())
		{
			EmotionComp->ApplyEmotionEffect(EffectType, EmotionDelta, DecayRate);
		}
	}
	else if (UEmotionComponent* EmotionComp = Cast<UEmotionComponent>(Target))
	{
		EmotionComp->ApplyEmotionEffect(EffectType, EmotionDelta, DecayRate);
	}

}

TArray<FEmotionParameter> UEmotionComponent::TryGetEmotion(UObject* Target)
{
	if (AActor* actor = Cast<AActor>(Target))
	{
		if (UEmotionComponent* EmotionComp = actor->FindComponentByClass<UEmotionComponent>())
		{
			return EmotionComp->GetEmotion();
		}
	}
	else if (UEmotionComponent* EmotionComp = Cast<UEmotionComponent>(Target))
	{
		return EmotionComp->GetEmotion();
	}
	TArray<FEmotionParameter> EmptyResult;
	return EmptyResult;
}