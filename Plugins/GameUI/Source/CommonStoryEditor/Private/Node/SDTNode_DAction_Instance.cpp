#include "Node/SDTNode_DAction_Instance.h"


void USDTNode_DAction_Instance::GetPersistObjects(TSet<UObject*>& Objects)
{
	if (ActionInstance)
	{
		Objects.Add(ActionInstance);
	}
}