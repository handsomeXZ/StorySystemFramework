
#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "StoryGraph.generated.h"

class UObject;

UCLASS(MinimalAPI)
class UStoryGraph : public UEdGraph
{
	GENERATED_UCLASS_BODY()

	virtual void OnCreated();
	virtual void OnNodesPasted(const FString& ImportStr);
	virtual void UpdateAsset(int32 UpdateFlags = 0) {}

	void UpdateAsset_Internal(int32 UpdateFlags = 0);

	bool IsLocked() const;
	void LockUpdates();
	void UnlockUpdates();
	void OnSave();

private:

	/** if set, graph modifications won't cause updates in internal tree structure
	 *  flag allows freezing update during heavy changes like pasting new nodes 
	 */
	uint32 bLockUpdates : 1;

};

