#include "CommonGame/DialogueTableRaw.h"
#include "Manager/DialogueManager.h"

FCommonDialogueTableRawBase::FCommonDialogueTableRawBase()
{
	SourceMode = EDialogueSourceMode::NPC;
	TimeWaitForSkip = 0;
}