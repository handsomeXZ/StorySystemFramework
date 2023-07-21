
#pragma once

#include "CoreMinimal.h"
#include "GameFramework\GameModeBase.h"

#include "TYLGameMode.generated.h"

class UExperienceDefinition;
struct FPrimaryAssetId;
/**
 * ATYLGameMode
 *
 *	The base game mode class used by this project.
 *	Pair this with a ATYLGameState
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base game mode class used by this project."))
class ATYLGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ATYLGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AGameModeBase interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;
	//~End of AGameModeBase interface

protected:

	void OnExperienceLoaded(const UExperienceDefinition* CurrentExperience);
	bool IsExperienceLoaded() const;

	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource);

	void HandleMatchAssignmentIfNotExpectingOne();
};