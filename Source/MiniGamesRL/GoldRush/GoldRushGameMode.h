// Guchan Alkan - Licensed under GPLv3


#pragma once

#include "CoreMinimal.h"
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

protected:
	virtual void BeginPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	
private:
	UFUNCTION()
	void SpawnObstacle();

	UFUNCTION()
	void SpawnCollectible();

	UPROPERTY(EditAnywhere, Category = "GoldRush|Classes")
	TSubclassOf<AActor> ObstacleClass;

	UPROPERTY(EditAnywhere, Category = "GoldRush|Classes")
	TSubclassOf<AActor> PlayerClass;

	UPROPERTY(EditAnywhere, Category = "GoldRush|Classes")
	TSubclassOf<AActor> CollectibleClass;

	UPROPERTY()
	AGoldRushLearningManager* LearningManager;

	FTimerHandle SpawnTimerHandle;

	FTimerHandle CollectibleSpawnTimerHandle;
};
