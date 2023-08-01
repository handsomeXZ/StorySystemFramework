#include "Node/SDTNode_DSelector_Instance.h"

void USDTNode_DSelector_Instance::GetPersistObjects(TSet<UObject*>& Objects)
{
	if (ActionInstance)
	{
		Objects.Add(ActionInstance);
	}
}