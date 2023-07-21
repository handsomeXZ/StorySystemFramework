#include "Manager/EventTagManager.h"

UEventTagManager::UEventTagManager()
{

}

void UEventTagManager::Initialize(FSubsystemCollectionBase& Collection)
{

}

void UEventTagManager::Deinitialize()
{

}

void UEventTagManager::UpdateTag(const FGameplayTag& Tag)
{
	EventTagContainer.AddTagFast(Tag);
}

void UEventTagManager::UpdateTags(const FGameplayTagContainer& Container)
{
	EventTagContainer.AppendTags(Container);
}

bool UEventTagManager::HasEventTag(const FGameplayTag& Tag, bool bIsExact)
{
	if (bIsExact)
	{
		return EventTagContainer.HasTagExact(Tag);
	}
	else
	{
		return EventTagContainer.HasTag(Tag);
	}
}

bool UEventTagManager::HasEventTags(const FGameplayTagContainer& Container, bool bIsExact, bool bIsAllMatching)
{
	if (bIsExact)
	{
		if (bIsAllMatching)
		{
			return EventTagContainer.HasAllExact(Container);
		}
		else
		{
			return EventTagContainer.HasAnyExact(Container);
		}
	}
	else
	{
		if (bIsAllMatching)
		{
			return EventTagContainer.HasAll(Container);
		}
		else
		{
			return EventTagContainer.HasAny(Container);
		}
	}
}