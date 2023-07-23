
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Manager/DialogueManager.h"
#include "CommonDialogueActorComponent.generated.h"

struct FPrimaryAssetId;
struct FInputBindingHandle;
class UWidgetComponent;
class UCommonDialogue_WidgetComponent;

UCLASS(Blueprintable, Meta = (DisplayName = "Dialogue NPC Component", BlueprintSpawnableComponent))
class COMMONSTORYRUNTIME_API UCommonDialogueActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCommonDialogueActorComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category = "Dialogue|Actor")
	static UCommonDialogueActorComponent* FindDialogueActorComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UCommonDialogueActorComponent>() : nullptr); }

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Initialize(FGameplayTag InIdentityTag, TSubclassOf<UCommonDialogue_WidgetComponent> InWidgetCompClass, TSubclassOf<UUserWidget> InWidgetClass,
	FVector InRelativeLocationIn = FVector(0, 0, 0), bool InbOverrideSizeIn = false, FVector2D InSizeOverrideIn = FVector2D(0.0, 0.0));

	UFUNCTION(BlueprintCallable)
	bool ActivateDialogueInteractive();

	/**
	 * Bind some delegates to the dialog Manager
	 */
	UFUNCTION(BlueprintCallable)
	void Register();

	UFUNCTION(BlueprintCallable)
	void UnRegister();
	 
	//UFUNCTION(BlueprintCallable)
	void ShowTemporarily(FStoryDialogueContext& Context);
	//UFUNCTION(BlueprintCallable)
	void Immediate_DiscardShowing();

	void OnReceiveDialogues(FStoryDialogueContext& Context); // Received Dialogue

private:
	void RegisterReceive();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue|Identity", meta = (Categories = "Gameplay.Role"))
	FGameplayTag IdentityTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue|Widget")
	TSubclassOf<UCommonDialogue_WidgetComponent> WidgetCompClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue|Widget", meta = (EditCondition = "WidgetCompClass!=nullptr"))
	TSubclassOf<UUserWidget> WidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue|Widget", meta = (EditCondition = "WidgetCompClass!=nullptr"))
	FVector RelativeLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue|Widget", meta = (EditCondition = "WidgetCompClass!=nullptr"))
	bool bOverrideSize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue|Widget", meta = (EditCondition = "bOverrideSize&&WidgetComp!=nullptr"))
	FVector2D SizeOverride;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	TObjectPtr<UCommonDialogue_WidgetComponent> WidgetComp;
};