// Fill out your copyright notice in the Description page of Project Settings.

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

private:
	UFUNCTION()
	void SpawnObstacle();

	UPROPERTY(EditAnywhere, Category = "GoldRush|Classes")
	TSubclassOf<AActor> ObstacleClass;

	UPROPERTY(EditAnywhere, Category = "GoldRush|Classes")
	TSubclassOf<AActor> PlayerClass;

	UPROPERTY(EditAnywhere, Category = "GoldRush")
	TArray<AActor*> Obstacles;

	FTimerHandle SpawnTimerHandle;
};
