// Copyright Epic Games, Inc. All Rights Reserved.

//#pragma once
//
//#include "CoreMinimal.h"
//#include "Engine/World.h"
//#include "GameFreatures/GameFeatureAction_WorldActionBase.h"
//#include "GameFeaturesSubsystem.h"
//
//#include "GameFeatureAction_AddDialogueComponents.generated.h"
//
//class AActor;
//class UActorComponent;
//class UGameFrameworkComponentManager;
//class UGameInstance;
//class UCommonDialogueActorComponent;
//class UDialogueDataView;
//struct FGameplayTag;
//struct FComponentRequestHandle;
//struct FWorldContext;
//
//USTRUCT()
//struct COMMONSTORYRUNTIME_API FExtraData
//{
//	GENERATED_BODY()
//	
//public:
//	UPROPERTY(EditAnywhere, Category = Component)
//	FGameplayTag Identify;
//
//	UPROPERTY(EditAnywhere, Category = Component)
//	// It seems more logical to use a hard reference.
//	TArray<TSoftObjectPtr<const UDialogueDataView>> DialogueDataView;
//};
//
//USTRUCT()
//struct COMMONSTORYRUNTIME_API FDialogueEntryData
//{
//	GENERATED_BODY()
//public:
//	UPROPERTY(EditAnywhere, Category = Component)
//	TSoftClassPtr<UUserWidget> DialogueEntryClass;
//
//	UPROPERTY(EditAnywhere, Category = Component)
//	FVector RelativeLocation;
//
//	UPROPERTY(EditAnywhere, Category = Component)
//	bool bOverrideSize;
//
//	UPROPERTY(EditAnywhere, Category = Component, meta = (EditCondition = "bOverrideSize"))
//	FVector2D SizeOverride;
//};
//
//USTRUCT()
//struct COMMONSTORYRUNTIME_API FGameFeatureComponentWithDataEntry
//{
//	GENERATED_BODY()
//public:
//	// The base actor class to add a component to
//	UPROPERTY(EditAnywhere, Category = Component, meta=(AllowAbstract="True"))
//	TSoftClassPtr<AActor> ActorClass;
//
//	// The UCommonDialogueActorComponent class to add to the specified type of actor
//	UPROPERTY(EditAnywhere, Category = Component)
//	TSoftClassPtr<UCommonDialogueActorComponent> ComponentClass;
//
//	UPROPERTY(EditAnywhere, Category = Component)
//	FDialogueEntryData DialogueEntry;
//
//	UPROPERTY(EditAnywhere, Category = Component, meta = (Categories = "Gameplay.Role"))
//	TArray<FExtraData> ExtraData;
//
//	// Should this component be added for clients
//	UPROPERTY(EditAnywhere, Category= Component)
//	uint8 bClientComponent:1;
//
//	// Should this component be added on servers
//	UPROPERTY(EditAnywhere, Category= Component)
//	uint8 bServerComponent:1;
//
//	FGameFeatureComponentWithDataEntry()
//		: bClientComponent(true)
//		, bServerComponent(true)
//	{
//	}
//};	
//
////////////////////////////////////////////////////////////////////////
//// UGameFeatureAction_AddComponents
//
///**
// * Adds actor<->component spawn requests to the component manager
// *
// * @see UGameFrameworkComponentManager
// */
//UCLASS(MinimalAPI, meta = (DisplayName = "Add DialogueComponents With Data"))
//class UGameFeatureAction_AddDialogueComponents final : public UGameFeatureAction_WorldActionBase
//{
//	GENERATED_BODY()
//
//public:
//	//~UGameFeatureAction interface
//	/*virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) overrid;*/
//	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
//#if WITH_EDITORONLY_DATA
//	virtual void AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData) override;
//#endif
//	//~End of UGameFeatureAction interface
//
//	//~UObject interface
//#if WITH_EDITOR
//	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
//#endif
//	//~End of UObject interface
//
//	/** List of components to add to gameplay actors when this game feature is enabled */
//	UPROPERTY(EditAnywhere,Category = Component, meta=(TitleProperty="{ActorClass} -> {ComponentClass}"))
//	TArray<FGameFeatureComponentWithDataEntry> ComponentList;
//
//private:
//	struct FContextHandles
//	{
//		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequestHandles;
//		TMap<UClass*, TSet<FObjectKey>> ComponentClassToComponentInstanceMap;
//	};
//
//	//~ Begin UGameFeatureAction_WorldActionBase interface
//	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
//	//~ End UGameFeatureAction_WorldActionBase interface
//
//	/*void AddToWorld(const FWorldContext& WorldContext, FContextHandles& Handles);*/
//
//	/*void HandleGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext ChangeContext);*/
//
//	void HandleActorComponentExtension(AActor* Actor, FName EventName, 
//		TSubclassOf<UCommonDialogueActorComponent> ComponentClass, TSubclassOf<UUserWidget> WidgetClass, const TArray<FExtraData> ExtraData,
//		FDialogueEntryData DialogueEntry, FGameFeatureStateChangeContext ChangeContext);
//
//	void CreateComponentOnInstance(AActor* ActorInstance, 
//	TSubclassOf<UCommonDialogueActorComponent> ComponentClass, TSubclassOf<UUserWidget> WidgetClass, const TArray<FExtraData>& ExtraData,
//	const FDialogueEntryData& DialogueEntry, FContextHandles& Handles);
//
//	TMap<FGameFeatureStateChangeContext, FContextHandles> ContextHandles;
//};
