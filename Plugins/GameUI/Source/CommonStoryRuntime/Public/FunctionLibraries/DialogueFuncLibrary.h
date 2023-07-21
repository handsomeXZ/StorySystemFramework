// Copyright Jin Taojie, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonGame/CommonDialogueActorComponent.h"
#include "DialogueFuncLibrary.generated.h"

struct FGameplayTag;

UCLASS()
class COMMONSTORYRUNTIME_API UDialogueFuncLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UDialogueFuncLibrary() { }

	UFUNCTION(BlueprintCallable, Category = "Global Dialogue Function", meta = (HidePin = "ContextObject", DefaultToSelf = "ContextObject"))
	static bool ActivateDialogueEvent(const UObject* ContextObject = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Global Get IdentifyComponent Tag")
	static FGameplayTag GetTargetIdentify(const AActor* Target = nullptr);

	
};