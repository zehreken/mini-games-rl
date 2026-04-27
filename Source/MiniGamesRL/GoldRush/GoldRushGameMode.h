// Guchan Alkan - Licensed under GPLv3


#pragma once

#include "CoreMinimal.h"
#include "GoldRushArenaManager.h"
#include "GameFramework/GameModeBase.h"
#include "GoldRush/GoldRushLearningManager.h"
#include "GoldRushGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MINIGAMESRL_API AGoldRushGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGoldRushGameMode();

	UFUNCTION()
	AGoldRushLearningManager* GetLearningManager();

	UPROPERTY(EditAnywhere, Category = "GoldRush|Classes")
	TSubclassOf<AActor> ArenaClass;

	UPROPERTY(EditAnywhere, Category = "GoldRush|Classes")
	TSubclassOf<AActor> ObstacleClass;

	UPROPERTY(EditAnywhere, Category = "GoldRush|Classes")
	TSubclassOf<AActor> PlayerClass;

	UPROPERTY(EditAnywhere, Category = "GoldRush|Classes")
	TSubclassOf<AActor> CollectibleClass;

	int32 GetCurrentPhaseId() const;

protected:
	virtual void BeginPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
private:
	UPROPERTY()
	AGoldRushLearningManager* LearningManager;

	UPROPERTY()
	TArray<AGoldRushArenaManager*> ArenaManagers;

	int32 PreviousChangeStepCount;

	int32 CurrentPhaseId;

	void OnCheckPhase(int32 StepCount);
};
