#pragma once

#include "CommonGame/TYLActivatableWidget.h"
#include "UObject/SoftObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "SettingHUDLayout.generated.h"

class UCommonActivatableWidget;
class UObject;


UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "Secondary HUD Layout", Category = "Secondary|HUD"))
class USettingHUDLayout : public UTYLActivatableWidget
{
	GENERATED_BODY()

public:

	USettingHUDLayout(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;

protected:
	void DiscardSelf();
};
