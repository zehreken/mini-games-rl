// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Tanks/TanksPlayer.h"
#include "GameFramework/GameModeBase.h"
#include "Learning/LearningManager.h"
#include "TanksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MINIGAMESRL_API ATanksGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATanksGameMode();

	UFUNCTION()
	ALearningManager* GetLearningManager();
	
protected:
	virtual void BeginPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

private:
	UPROPERTY()
	ALearningManager* LearningManager;

	UPROPERTY()
	ATanksPlayer* TanksPlayer;

	void OnCheckPhase(int32 StepCount) const;
};
