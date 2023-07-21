// Fill out your copyright notice in the Description page of Project Settings.


#include "EmotionalRuleModifier.h"

UEmotionalRuleModifier::UEmotionalRuleModifier(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bIsDirty(false)
{
	
}

TArray<FEmotionParameter>& UEmotionalRuleModifier::ExecuteModifier(TArray<FEmotionParameter>& EmotionParameters)
{
	if (bIsDirty)
	{
		Execute(EmotionParameters, EmotionModifyCache);
	}

	return EmotionModifyCache;
}