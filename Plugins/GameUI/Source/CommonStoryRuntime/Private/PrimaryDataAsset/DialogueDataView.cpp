#include "PrimaryDataAsset/DialogueDataView.h"



UDialogueDataView::UDialogueDataView(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

#if WITH_EDITOR 
EDataValidationResult UDialogueDataView::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(ValidationErrors), EDataValidationResult::Valid);

	if (EpisodeTag.IsValid() && !DialogueTable.IsNull())
	{
		Result = CombineDataValidationResults(Result, EDataValidationResult::Valid);
	}
	else
	{
		Result = CombineDataValidationResults(Result, EDataValidationResult::Invalid);
	}

	return Result;
}
#endif 


#if WITH_EDITORONLY_DATA
void UDialogueDataView::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

}
#endif // WITH_EDITORONLY_DATA