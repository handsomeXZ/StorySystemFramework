#pragma once

#include "CommonGame/TYLActivatableWidget.h"
#include "UObject/SoftObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "GameHUDLayout.generated.h"

class UCommonActivatableWidget;
class UObject;



UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "Game HUD Layout", Category = "Game|HUD"))
class UGameHUDLayout : public UTYLActivatableWidget
{
	GENERATED_BODY()

public:

	UGameHUDLayout(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;

protected:
	void PushOrRemoveSecondaryWidget();
	void PushOrRemoveDecryptionWidget();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> SecondaryMenuClass;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> DecryptionMenuClass;
private:
	
};
