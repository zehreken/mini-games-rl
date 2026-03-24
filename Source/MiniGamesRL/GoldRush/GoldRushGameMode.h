// Guchan Alkan - Licensed under GPLv3


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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
	
	UPROPERTY(EditAnywhere, Category = "GoldRush")
	TArray<AActor*> Obstacles;

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

	FTimerHandle SpawnTimerHandle;

	FTimerHandle CollectibleSpawnTimerHandle;
};
