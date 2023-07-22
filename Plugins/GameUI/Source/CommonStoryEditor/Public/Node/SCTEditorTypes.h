#pragma once

#include "CoreMinimal.h"

#include "SCTEditorTypes.generated.h"

// Maximum distance a drag can be off a node edge to require 'push off' from node
const int32 NodeDistance = 60;

namespace SCTClassUtils
{
	FString ClassToString(UClass* InClass);
}

UCLASS()
class USCTEditorTypes : public UObject
{
	GENERATED_UCLASS_BODY()

	static const FName PinCategory_Defualt;
	static const FName PinCategory_Entry;
	static const FName PinCategory_Chapter;
	static const FName PinCategory_Transition;
	
};

UCLASS()
class USDTEditorTypes : public UObject
{
	GENERATED_UCLASS_BODY()

	static const FName PinCategory_Defualt;
	static const FName PinCategory_Entry;
	static const FName PinCategory_Exit;
	static const FName PinCategory_Return;
	static const FName PinCategory_Continue;
	static const FName PinCategory_Content;
	static const FName PinCategory_Options;
	static const FName PinCategory_Selector;
	static const FName PinCategory_Action;
};
