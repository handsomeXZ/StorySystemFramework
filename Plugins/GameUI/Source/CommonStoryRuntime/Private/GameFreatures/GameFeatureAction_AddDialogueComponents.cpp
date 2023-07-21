// Copyright Epic Games, Inc. All Rights Reserved.

//#include "GameFreatures/GameFeatureAction_AddDialogueComponents.h"
//#include "Components/GameFrameworkComponentManager.h"
//#include "Components/WidgetComponent.h"
//#include "GameFeaturesSubsystemSettings.h"
//#include "Engine/AssetManager.h"
//#include "UObject/SoftObjectPtr.h"
//
//#include "CommonGame/IdentifyActorComponent.h"
//#include "CommonGame/CommonDialogueActorComponent.h"
//#include "CommonGame/CommonDialogue_WidgetComponent.h"
//
//#include "PrimaryDataAsset/DialogueDataView.h"
//
////@TODO: Just for log category
//#include "GameFeaturesSubsystem.h"
//
//#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_AddDialogueComponents)
//
//#define LOCTEXT_NAMESPACE "GameFeatures"
//
////////////////////////////////////////////////////////////////////////
//// UGameFeatureAction_AddDialogueComponents
//
////void UGameFeatureAction_AddDialogueComponents::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
////{
////	FContextHandles& Handles = ContextHandles.FindOrAdd(Context);
////
////	Handles.GameInstanceStartHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, 
////		&UGameFeatureAction_AddDialogueComponents::HandleGameInstanceStart, FGameFeatureStateChangeContext(Context));
////
////	ensure(Handles.ComponentRequestHandles.Num() == 0);
////
////	// Add to any worlds with associated game instances that have already been initialized
////	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
////	{
////		if (Context.ShouldApplyToWorldContext(WorldContext))
////		{
////			AddToWorld(WorldContext, Handles);
////		}
////	}
////}
//
//void UGameFeatureAction_AddDialogueComponents::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
//{
//	Super::OnGameFeatureDeactivating(Context);
//	
//	FContextHandles& Handles = ContextHandles.FindOrAdd(Context);
//
//	// Releasing the handles will also remove the components from any registered actors too
//	Handles.ComponentRequestHandles.Empty();
//}
//
//#if WITH_EDITORONLY_DATA
//void UGameFeatureAction_AddDialogueComponents::AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData)
//{
//	if (UAssetManager::IsValid())
//	{
//		for (const FGameFeatureComponentWithDataEntry& Entry : ComponentList)
//		{
//			if (Entry.bClientComponent)
//			{
//				AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, Entry.ComponentClass.ToSoftObjectPath().GetAssetPath());
//			}
//			if (Entry.bServerComponent)
//			{
//				AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateServer, Entry.ComponentClass.ToSoftObjectPath().GetAssetPath());
//			}
//		}
//	}
//}
//#endif
//
//#if WITH_EDITOR
//EDataValidationResult UGameFeatureAction_AddDialogueComponents::IsDataValid(TArray<FText>& ValidationErrors)
//{
//	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(ValidationErrors), EDataValidationResult::Valid);
//
//	int32 EntryIndex = 0;
//	for (const FGameFeatureComponentWithDataEntry& Entry : ComponentList)
//	{
//		if (Entry.ActorClass.IsNull())
//		{
//			Result = EDataValidationResult::Invalid;
//			ValidationErrors.Add(FText::Format(LOCTEXT("ComponentEntryHasNullActor", "Null ActorClass at index {0} in ComponentList"), FText::AsNumber(EntryIndex)));
//		}
//
//		if (Entry.DialogueEntry.DialogueEntryClass.IsNull())
//		{
//			Result = EDataValidationResult::Invalid;
//			ValidationErrors.Add(FText::Format(LOCTEXT("ComponentEntryHasNullDialogueEntry", "Null DialogueEntryClass at index {0} in ComponentList"), FText::AsNumber(EntryIndex)));
//		}
//
//		if (Entry.ComponentClass.IsNull())
//		{
//			Result = EDataValidationResult::Invalid;
//			ValidationErrors.Add(FText::Format(LOCTEXT("ComponentEntryHasNullComponent", "Null ComponentClass at index {0} in ComponentList"), FText::AsNumber(EntryIndex)));
//		}
//
//		++EntryIndex;
//	}
//
//	return Result;
//}
//#endif
//
//void UGameFeatureAction_AddDialogueComponents::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
//{
//	UWorld* World = WorldContext.World();
//	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
//	FContextHandles& Handles = ContextHandles.FindOrAdd(ChangeContext);
//
//	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
//	{
//		if (UGameFrameworkComponentManager* GFCM = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
//		{
//			const ENetMode NetMode = World->GetNetMode();
//			const bool bIsServer = NetMode != NM_Client;
//			const bool bIsClient = NetMode != NM_DedicatedServer;
//
//			UE_LOG(LogGameFeatures, Verbose, TEXT("Adding components for %s to world %s (client: %d, server: %d)"), *GetPathNameSafe(this), *World->GetDebugDisplayName(), bIsClient ? 1 : 0, bIsServer ? 1 : 0);
//
//			for (const FGameFeatureComponentWithDataEntry& Entry : ComponentList)
//			{
//				const bool bShouldAddRequest = (bIsServer && Entry.bServerComponent) || (bIsClient && Entry.bClientComponent);
//				if (bShouldAddRequest)
//				{
//					if (!Entry.ActorClass.IsNull())
//					{
//						UE_SCOPED_ENGINE_ACTIVITY(TEXT("Adding component to world %s (%s)"), *World->GetDebugDisplayName(), *Entry.ComponentClass.ToString());
//						TSubclassOf<UCommonDialogueActorComponent> ComponentClass = Entry.ComponentClass.LoadSynchronous();
//						TSubclassOf<UUserWidget> WidgetClass = Entry.DialogueEntry.DialogueEntryClass.LoadSynchronous();
//						if (ComponentClass)
//						{
//							/*Handles.ComponentRequestHandles.Add(GFCM->AddComponentRequest(Entry.ActorClass, ComponentClass));*/
//							TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = GFCM->AddExtensionHandler(
//								Entry.ActorClass,
//								UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleActorComponentExtension, ComponentClass, WidgetClass, Entry.ExtraData, Entry.DialogueEntry, ChangeContext));
//
//							Handles.ComponentRequestHandles.Add(ExtensionRequestHandle);
//
//						}
//						else if (!Entry.ComponentClass.IsNull())
//						{
//							UE_LOG(LogGameFeatures, Error, TEXT("[GameFeatureData %s]: Failed to load component class %s. Not applying component."), *GetPathNameSafe(this), *Entry.ComponentClass.ToString());
//						}
//					}
//				}
//			}
//		}
//	}
//
//
//}
//
////void UGameFeatureAction_AddDialogueComponents::AddToWorld(const FWorldContext& WorldContext, FContextHandles& Handles)
////{
////	UWorld* World = WorldContext.World();
////	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
////
////	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
////	{
////		if (UGameFrameworkComponentManager* GFCM = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
////		{
////			const ENetMode NetMode = World->GetNetMode();
////			const bool bIsServer = NetMode != NM_Client;
////			const bool bIsClient = NetMode != NM_DedicatedServer;
////
////			UE_LOG(LogGameFeatures, Verbose, TEXT("Adding components for %s to world %s (client: %d, server: %d)"), *GetPathNameSafe(this), *World->GetDebugDisplayName(), bIsClient ? 1 : 0, bIsServer ? 1 : 0);
////			
////			for (const FGameFeatureComponentWithDataEntry& Entry : ComponentList)
////			{
////				const bool bShouldAddRequest = (bIsServer && Entry.bServerComponent) || (bIsClient && Entry.bClientComponent);
////				if (bShouldAddRequest)
////				{
////					if (!Entry.ActorClass.IsNull())
////					{
////						UE_SCOPED_ENGINE_ACTIVITY(TEXT("Adding component to world %s (%s)"), *World->GetDebugDisplayName(), *Entry.ComponentClass.ToString());
////						TSubclassOf<UCommonDialogueActorComponent> ComponentClass = Entry.ComponentClass.LoadSynchronous();
////						if (ComponentClass)
////						{
////							Handles.ComponentRequestHandles.Add(GFCM->AddComponentRequest(Entry.ActorClass, ComponentClass));
////							TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = GFCM->AddExtensionHandler(
////							Entry.ActorClass, 
////								UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleActorComponentExtension, ));
////						}
////						else if (!Entry.ComponentClass.IsNull())
////						{
////							UE_LOG(LogGameFeatures, Error, TEXT("[GameFeatureData %s]: Failed to load component class %s. Not applying component."), *GetPathNameSafe(this), *Entry.ComponentClass.ToString());
////						}
////					}
////				}
////			}
////		}
////	}
////}
//
////void UGameFeatureAction_AddDialogueComponents::HandleGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext ChangeContext)
////{
////	if (FWorldContext* WorldContext = GameInstance->GetWorldContext())
////	{
////		if (ChangeContext.ShouldApplyToWorldContext(*WorldContext))
////		{
////			FContextHandles* Handles = ContextHandles.Find(ChangeContext);
////			if (ensure(Handles))
////			{
////				AddToWorld(*WorldContext, *Handles);
////			}
////		}
////	}
////}
//void UGameFeatureAction_AddDialogueComponents::HandleActorComponentExtension(AActor* Actor, FName EventName, 
//	TSubclassOf<UCommonDialogueActorComponent> ComponentClass, TSubclassOf<UUserWidget> WidgetClass, const TArray<FExtraData> ExtraData,
//	FDialogueEntryData DialogueEntry, FGameFeatureStateChangeContext ChangeContext)
//{
//	FContextHandles& Handles = ContextHandles.FindOrAdd(ChangeContext);
//	//if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved) || (EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved))
//	//{
//	//	RemoveWidgets(Actor, ActiveData);
//	//}
//	//else if
//	if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded) || (EventName == UGameFrameworkComponentManager::NAME_GameActorReady))
//	{
//		if (Actor->FindComponentByClass(UIdentifyActorComponent::StaticClass()))
//		{
//			CreateComponentOnInstance(Actor, ComponentClass, WidgetClass, ExtraData, DialogueEntry, Handles);
//		}
//	}
//}
//
//void UGameFeatureAction_AddDialogueComponents::CreateComponentOnInstance(AActor* ActorInstance, 
//	TSubclassOf<UCommonDialogueActorComponent> ComponentClass, TSubclassOf<UUserWidget> WidgetClass, const TArray<FExtraData>& ExtraData,
//	const FDialogueEntryData& DialogueEntry, FContextHandles& Handles)
//{
//	check(ActorInstance);
//	check(ComponentClass);
//
//	if (!ComponentClass->GetDefaultObject<UCommonDialogueActorComponent>()->GetIsReplicated() || ActorInstance->GetLocalRole() == ROLE_Authority)
//	{
//		UCommonDialogueActorComponent* NewComp = NewObject<UCommonDialogueActorComponent>(ActorInstance, ComponentClass, ComponentClass->GetFName());
//		UCommonDialogue_WidgetComponent* NewWidgetComp = NewObject<UCommonDialogue_WidgetComponent>(ActorInstance);
//		
//
//		// Initialize the Widget component
//		NewWidgetComp->SetWidgetClass(WidgetClass);
//		NewWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
//		if (DialogueEntry.bOverrideSize)
//		{
//			NewWidgetComp->SetDrawSize(DialogueEntry.SizeOverride);
//		}
//		else
//		{
//			NewWidgetComp->SetDrawSize(FVector2D(0.0, 0.0));
//		}
//		NewWidgetComp->SetRelativeLocation(DialogueEntry.RelativeLocation);
//
//		// Initialize the Dialogue component
//		UIdentifyActorComponent* IdentifyComp = Cast<UIdentifyActorComponent>(ActorInstance->FindComponentByClass(UIdentifyActorComponent::StaticClass()));
//		NewComp->Initialize(NewWidgetComp, IdentifyComp->IdentityTag, IdentifyComp->DialogueGroupTag);
//
//		UAssetManager& AssetManager = UAssetManager::Get();
//		for (const FExtraData& data : ExtraData)
//		{
//			if (data.Identify == NewComp->IdentityTag)
//			{
//				for (const TSoftObjectPtr<const UDialogueDataView>& ViewPtr : data.DialogueDataView)
//				{
//					if (const UDialogueDataView* view = ViewPtr.LoadSynchronous())
//					{
//						FDialogueData dialogueData = FDialogueData(view->bEnableInput, view->DialogueTable, view->InputAction, view->InputConfig);
//						NewComp->AddDialogueData(view->EpisodeTag, dialogueData);
//					}
//				}
//			}
//			
//		}
//		
//		TSet<FObjectKey>& ComponentInstances = Handles.ComponentClassToComponentInstanceMap.FindOrAdd(*ComponentClass);
//		ComponentInstances.Add(NewComp);
//		ComponentInstances.Add(NewWidgetComp);
//
//		
//		NewWidgetComp->SetupAttachment(ActorInstance->GetRootComponent());
//		if (USceneComponent* NewSceneComp = Cast<USceneComponent>(NewComp))
//		{
//			NewSceneComp->SetupAttachment(ActorInstance->GetRootComponent());
//		}
//		
//		NewComp->RegisterComponent();
//		NewWidgetComp->RegisterComponent();
//	}
//}
////////////////////////////////////////////////////////////////////////
//
//#undef LOCTEXT_NAMESPACE

