// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GoldRush/GoldRushPlayer.h"
#include "GameFramework/Actor.h"
#include "GoldRushArenaManager.generated.h"

UCLASS()
class MINIGAMESRL_API AGoldRushArenaManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGoldRushArenaManager();

	void InitArena(int32 ArenaIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void SpawnObstacle();

	UFUNCTION()
	void SpawnCollectible();
	
	UPROPERTY()
	AGoldRushPlayer* Player;

	FVector ArenaOffset;

	FTimerHandle SpawnTimerHandle;

	FTimerHandle CollectibleSpawnTimerHandle;

	UPROPERTY(EditAnywhere)
	int32 Seed = 42;

	FRandomStream RandomStream;
};
