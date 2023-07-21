// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonGame/TYLActivatableWidget.h"
#include "SmallMap_Widget.generated.h"


UCLASS(Abstract, Blueprintable)
class GAMEUI_API USmallMap_Widget : public UTYLActivatableWidget
{	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable)
	void ResetSmallMap(UTexture2D* SmallMapIcon, FVector2D RangeSize, float Scale);

	UFUNCTION(BlueprintImplementableEvent, Category = "SmallMapWidget", meta = (DisplayName = "BP_ResetSmallMap"))
	void BP_ResetSmallMap(UTexture2D* SmallMapIcon, FVector2D RangeSize, float Scale);



};
