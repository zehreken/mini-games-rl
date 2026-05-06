// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Runners/RunnersPlayer.h"
#include "GameFramework/GameModeBase.h"
#include "RunnersGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MINIGAMESRL_API ARunnersGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARunnersGameMode();

	UFUNCTION()
	ALearningManager* GetLearningManager();

protected:
	virtual void BeginPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

private:
	UPROPERTY()
	ALearningManager* LearningManager;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARunnersPlayer> PlayerClass;

	void OnCheckPhase(int32 StepCount) const;
};
