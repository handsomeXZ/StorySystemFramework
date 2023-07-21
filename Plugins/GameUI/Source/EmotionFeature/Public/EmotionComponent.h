// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EmotionalRuleModifier.h"
#include "EmotionComponent.generated.h"

UENUM(BlueprintType)
enum class EEmotionEffectType : uint8
{
	Single,
	Related,
	InRange,
};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EMOTIONFEATURE_API UEmotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEmotionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//UFUNCTION(BlueprintCallable)
	//void RegisterEmotionRlue(UEmotionalRuleModifier* RuleModifier);
	//UFUNCTION(BlueprintCallable)
	//void ActivateEmotionRlue();
	//UFUNCTION(BlueprintCallable)
	//void DeActivateEmotionRlue();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ApplyRuleModifier(UEmotionalRuleModifier* RuleModifierIn);
	UFUNCTION(BlueprintCallable)
	void ActivateRuleModifier() { bEnableRlueModifier = true; }
	UFUNCTION(BlueprintCallable)
	void InActivateRuleModifier() { bEnableRlueModifier = false; }

	UFUNCTION(BlueprintCallable)
	void ApplyEmotionEffect(EEmotionEffectType EffectType, FEmotionParameter EmotionDelta, float DecayRate = 1.f);

	UFUNCTION(BlueprintCallable)
	TArray<FEmotionParameter> GetEmotion();


	UFUNCTION(BlueprintCallable, meta = (HideSelfPin))
	static void TryApplyRuleModifier(UObject* Target, UEmotionalRuleModifier* RuleModifierIn);
	UFUNCTION(BlueprintCallable, meta = (HideSelfPin))
	static void TryActivateRuleModifier(UObject* Target);
	UFUNCTION(BlueprintCallable, meta = (HideSelfPin))
	static void TryInActivateRuleModifier(UObject* Target);
	UFUNCTION(BlueprintCallable, meta = (HideSelfPin))
	static void TryApplyEmotionEffect(UObject* Target, EEmotionEffectType EffectType, FEmotionParameter EmotionDelta, float DecayRate = 1.f);
	UFUNCTION(BlueprintCallable, meta = (HideSelfPin))
	static TArray<FEmotionParameter> TryGetEmotion(UObject* Target);

protected:
	void UpdateEmotion(FEmotionParameter& Target, FEmotionParameter EmotionDelta, float DecayRate);

	void ApplyEffect_Single(FEmotionParameter EmotionDelta, float DecayRate = 1.f);
	void ApplyEffect_Related(FEmotionParameter EmotionDelta, float DecayRate = 1.f);
	void ApplyEffect_InRange(FEmotionParameter EmotionDelta, float DecayRate = 1.f);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<AActor>> RelatedTarget;

private:
	bool bEnableRlueModifier;
	FEmotionParameter AlertValue;
	FEmotionParameter EcstaticValue;
	FEmotionParameter AdmirationValue;
	FEmotionParameter FrightValue;
	TObjectPtr<UEmotionalRuleModifier> RuleModifier = nullptr;
};
