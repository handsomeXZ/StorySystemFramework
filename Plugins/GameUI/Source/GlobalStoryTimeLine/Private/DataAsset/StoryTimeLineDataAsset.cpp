#include "DataAsset/StoryTimeLineDataAsset.h"



UStoryTimeLineDataAsset::UStoryTimeLineDataAsset()
{
}

#if WITH_EDITOR 
EDataValidationResult UStoryTimeLineDataAsset::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(ValidationErrors), EDataValidationResult::Valid);

	return Result;
}
#endif 


#if WITH_EDITORONLY_DATA
void UStoryTimeLineDataAsset::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

}
#endif // WITH_EDITORONLY_DATA