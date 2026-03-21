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

protected:
	virtual void BeginPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

public:
	UPROPERTY(EditAnywhere, Category = "GoldRush")
	TArray<AActor*> Obstacles;
	
private:
	UFUNCTION()
	void SpawnObstacle();

	UPROPERTY(EditAnywhere, Category = "GoldRush|Classes")
	TSubclassOf<AActor> ObstacleClass;

	UPROPERTY(EditAnywhere, Category = "GoldRush|Classes")
	TSubclassOf<AActor> PlayerClass;

	FTimerHandle SpawnTimerHandle;
};
