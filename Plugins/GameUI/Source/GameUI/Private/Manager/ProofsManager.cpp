#include "Manager/ProofsManager.h"

UProofsManager::UProofsManager()
{
	// ...
}

void UProofsManager::Initialize(FSubsystemCollectionBase& Collection)
{
	if (!DefaultProofsAsset.IsNull())
	{
		if (const UCommonProofDefinition* CommonProofs = DefaultProofsAsset.LoadSynchronous())
		{
			for (const FCommonProofItem& item : CommonProofs->ProofItems)
			{
				ProofsList.Add(item.ProofItemTag, item);
			}
		}
	}
}

void UProofsManager::Deinitialize()
{
	// ...
}

bool UProofsManager::CollectProof(FGameplayTag ProofItemTag)
{
	if (FCommonProofItem* item = ProofsList.Find(ProofItemTag))
	{
		if (!CollectedProofTags.Find(ProofItemTag))
		{
			CollectedProofs.Add(*item);
			CollectedProofTags.Add(ProofItemTag);
		}
		return true;
	}
	else
	{
		return false;
	}
}