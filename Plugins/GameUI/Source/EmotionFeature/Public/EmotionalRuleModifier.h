// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EmotionalRuleModifier.generated.h"

UENUM(BlueprintType)
enum class EEmotionType : uint8
{
	// Low
	Alert,
	Ecstatic,
	Admiration,
	Rage,
	// Advanced


};

USTRUCT(BlueprintType)
struct FEmotionParameter
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEmotionType EmotionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IntensityValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Priority;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class EMOTIONFEATURE_API UEmotionalRuleModifier : public UObject
{
	GENERATED_UCLASS_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ExecuteModifier"))
	void Execute(const TArray<FEmotionParameter>& EmotionParameters, TArray<FEmotionParameter>& Result);
	
	TArray<FEmotionParameter>& ExecuteModifier(TArray<FEmotionParameter>& EmotionParameters);
	void MarkDirty() { bIsDirty = true; }

private:
	bool bIsDirty;
	TArray<FEmotionParameter> EmotionModifyCache;

};
