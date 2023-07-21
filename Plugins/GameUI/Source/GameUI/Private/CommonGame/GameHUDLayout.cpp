#include "CommonGame/GameHUDLayout.h"
#include "GameUIManagerSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "GameUIPolicy.h"
#include "CommonLocalPlayer.h"
#include "NativeGameplayTags.h"
#include "UITag.h"
#include "Input\CommonUIInputTypes.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MENU, "UI.Layer.Menu");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_ACTION_SECONDARY, "UI.Action.Secondary");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_ACTION_DECRYPTION, "UI.Action.Decryption");

UGameHUDLayout::UGameHUDLayout(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UGameHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterUIActionBinding(FBindUIActionArgs(FUIActionTag::ConvertChecked(TAG_UI_ACTION_SECONDARY), false, FSimpleDelegate::CreateUObject(this, &ThisClass::PushOrRemoveSecondaryWidget)));
	RegisterUIActionBinding(FBindUIActionArgs(FUIActionTag::ConvertChecked(TAG_UI_ACTION_DECRYPTION), false, FSimpleDelegate::CreateUObject(this, &ThisClass::PushOrRemoveDecryptionWidget)));
}

void UGameHUDLayout::PushOrRemoveSecondaryWidget()
{
	if (ensure(!SecondaryMenuClass.IsNull()))
	{
		UCommonUIExtensions::PushStreamedContentToLayer_ForPlayer(GetOwningLocalPlayer(), TAG_UI_LAYER_MENU, SecondaryMenuClass);
	}
}

void UGameHUDLayout::PushOrRemoveDecryptionWidget()
{
	if (ensure(!DecryptionMenuClass.IsNull()))
	{
		UCommonUIExtensions::PushStreamedContentToLayer_ForPlayer(GetOwningLocalPlayer(), TAG_UI_LAYER_MENU, DecryptionMenuClass);
	}
}