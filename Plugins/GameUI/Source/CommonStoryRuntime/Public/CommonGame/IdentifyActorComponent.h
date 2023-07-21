// Copyright Jin Taojie, All Rights Reserved.

//#pragma once
//
//#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
//#include "GameplayTagContainer.h"
//#include "IdentifyActorComponent.generated.h"
//
//
//UCLASS(Blueprintable)
//class COMMONSTORYRUNTIME_API UIdentifyActorComponent : public UActorComponent
//{
//	GENERATED_BODY()
//
//public:
//	UIdentifyActorComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
//
//
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, SimpleDisplay, Category="GameplayTag", meta = (Categories = "Gameplay.Role"))
//	FGameplayTag IdentityTag;
//
//protected:
//	// Called when the game starts
//	virtual void BeginPlay() override;
//	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
//
//};