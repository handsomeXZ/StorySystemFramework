#include "CommonGame/SmallMap_Widget.h"
#include "GameMapManagerSubsystem.h"

void USmallMap_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UGameMapManagerSubsystem* MapManager = GetWorld()->GetGameInstance()->GetSubsystem<UGameMapManagerSubsystem>())
	{
		UGameMapManagerSubsystem::FSmallMapChangedHandler& handler = MapManager->GetHandler_OnSmallMapChanged();
		handler.BindUObject(this, &ThisClass::ResetSmallMap);
	}
}

void USmallMap_Widget::NativeDestruct()
{
	if (UGameMapManagerSubsystem* MapManager = GetWorld()->GetGameInstance()->GetSubsystem<UGameMapManagerSubsystem>())
	{
		UGameMapManagerSubsystem::FSmallMapChangedHandler handler = MapManager->GetHandler_OnSmallMapChanged();
		handler.Unbind();
	}
	Super::NativeDestruct();
}

void USmallMap_Widget::ResetSmallMap(UTexture2D* SmallMapIcon, FVector2D RangeSize, float Scale)
{
	BP_ResetSmallMap(SmallMapIcon, RangeSize, Scale);
}