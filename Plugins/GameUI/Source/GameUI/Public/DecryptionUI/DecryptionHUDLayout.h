#pragma once

#include "CommonGame\TYLActivatableWidget.h"
#include "UObject/SoftObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "DecryptionHUDLayout.generated.h"

class UCommonActivatableWidget;
class UObject;

UCLASS(Abstract, BlueprintType, Blueprintable, Meta = (DisplayName = "Decryption HUD Layout", Category = "Decryption|HUD"))
class UDecryptionHUDLayout : public UTYLActivatableWidget
{
	GENERATED_BODY()

public:

	UDecryptionHUDLayout(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;

protected:
	void DiscardSelf();
};
