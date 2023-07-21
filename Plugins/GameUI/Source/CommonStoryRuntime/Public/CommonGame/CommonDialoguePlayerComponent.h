
#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "GameplayTagContainer.h"
#include "Manager/DialogueManager.h"
#include "CommonDialoguePlayerComponent.generated.h"

class UCommonActivatableWidget;
struct FStoryDialogueContext;

UCLASS(Blueprintable, Meta = (DisplayName = "Dialogue Player Component", BlueprintSpawnableComponent))
class COMMONSTORYRUNTIME_API UCommonDialoguePlayerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UCommonDialoguePlayerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category = "Dialogue|Player")
	static UCommonDialoguePlayerComponent* FindDialoguePlayerComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UCommonDialoguePlayerComponent>() : nullptr); }

	/**
	 * Bind some delegates to the dialog Manager.
	 */
	UFUNCTION(BlueprintCallable)
	void Register();

	UFUNCTION(BlueprintCallable)
	void UnRegister();


	void OnReceiveDialogues(FStoryDialogueContext& DialogueContext); // Received Dialogue

	void ShowDialogueBox(FStoryDialogueContext& DialogueContext);
	void ShowDialogueOptionsBox(FStoryDialogueContext& DialogueContext);
	void DiscardShowing(TObjectPtr<UCommonActivatableWidget>& LocalPlayerDialogueWidget);

	void Immediate_DiscardShowing();

	void PushLocalPlayerDialogueWidgetToLayer(TObjectPtr<UCommonActivatableWidget>& LocalPlayerDialogueEntryWidgetIn, TSubclassOf<UCommonActivatableWidget> PlayerDialogueEntryClassIn);



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dialogue, meta = (Categories = "UI.Layer"))
	FGameplayTag LayerTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dialogue)
	TSubclassOf<UCommonActivatableWidget> PlayerDialogueOptionClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dialogue)
	TSubclassOf<UCommonActivatableWidget> PlayerDialogueEntryClass;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	//UPROPERTY()
	//TObjectPtr<UCommonActivatableWidget> LocalPlayerDialogueOptionWidget;
	//UPROPERTY()
	//TObjectPtr<UCommonActivatableWidget> LocalPlayerDialogueEntryWidget;
	UPROPERTY()
	TObjectPtr<UCommonActivatableWidget> LocalPlayerDialogueWidget;
};