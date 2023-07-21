#include "DecryptionUI/DecryptionHUDLayout.h"
#include "GameUIManagerSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "GameUIPolicy.h"
#include "CommonLocalPlayer.h"
#include "NativeGameplayTags.h"
#include "UITag.h"
#include "Input\CommonUIInputTypes.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_ACTION_DECRYPTIONESCAPE, "UI.Action.DecryptionEscape");

UDecryptionHUDLayout::UDecryptionHUDLayout(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UDecryptionHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterUIActionBinding(FBindUIActionArgs(FUIActionTag::ConvertChecked(TAG_UI_ACTION_DECRYPTIONESCAPE), false, FSimpleDelegate::CreateUObject(this, &ThisClass::DiscardSelf)));
}

void UDecryptionHUDLayout::DiscardSelf()
{
	DeactivateWidget();
}