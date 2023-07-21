#include "CommonGame/SettingHUDLayout.h"
#include "GameUIManagerSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "GameUIPolicy.h"
#include "CommonLocalPlayer.h"
#include "NativeGameplayTags.h"
#include "UITag.h"
#include "Input\CommonUIInputTypes.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_ACTION_SECONDARYESCAPE, "UI.Action.SecondaryEscape");

USettingHUDLayout::USettingHUDLayout(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void USettingHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterUIActionBinding(FBindUIActionArgs(FUIActionTag::ConvertChecked(TAG_UI_ACTION_SECONDARYESCAPE), false, FSimpleDelegate::CreateUObject(this, &ThisClass::DiscardSelf)));
}

void USettingHUDLayout::DiscardSelf()
{
	DeactivateWidget();
}