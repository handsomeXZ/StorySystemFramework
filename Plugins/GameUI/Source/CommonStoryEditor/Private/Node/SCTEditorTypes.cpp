#include "Node/SCTEditorTypes.h"

#include "UObject/NameTypes.h"

const FName USCTEditorTypes::PinCategory_Defualt("DefualtNode");
const FName USCTEditorTypes::PinCategory_Entry("EntryNode");
const FName USCTEditorTypes::PinCategory_Chapter("ChapterNode");
const FName USCTEditorTypes::PinCategory_Transition("TransitionNode");

USCTEditorTypes::USCTEditorTypes(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}


const FName USDTEditorTypes::PinCategory_Defualt("DefualtNode");
const FName USDTEditorTypes::PinCategory_Entry("EntryNode");
const FName USDTEditorTypes::PinCategory_Exit("ExitNode");
const FName USDTEditorTypes::PinCategory_Return("ReturnNode");
const FName USDTEditorTypes::PinCategory_Continue("ContinueNode");
const FName USDTEditorTypes::PinCategory_Content("ContentNode");
const FName USDTEditorTypes::PinCategory_Options("OptionsNode");
const FName USDTEditorTypes::PinCategory_Selector("SelectorNode");
const FName USDTEditorTypes::PinCategory_Action("ActionNode");
USDTEditorTypes::USDTEditorTypes(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}
