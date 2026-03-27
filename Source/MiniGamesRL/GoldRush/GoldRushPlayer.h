// Guchan Alkan - Licensed under GPLv3


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoldRushPlayer.generated.h"

class UTextureSwapComponent;

UCLASS()
class MINIGAMESRL_API AGoldRushPlayer : public APawn
{
	GENERATED_BODY()

public:
	AGoldRushPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void ResetAgent();
	void Move(float Direction);
	void MissObject();

	UPROPERTY(VisibleInstanceOnly, Category = "GoldRush")
	TArray<AActor*> Obstacles;

	UPROPERTY(VisibleInstanceOnly, Category = "GoldRush")
	TArray<AActor*> Collectibles;

	// True when player is hit by an obstacle
	UPROPERTY(VisibleInstanceOnly, Category = "GoldRush")
	bool bWasHit = false;

	// True when player collects a collectible
	UPROPERTY(VisibleInstanceOnly, Category = "GoldRush")
	bool bHasCollected = false;

	// True when an obstacle is avoided
	UPROPERTY(VisibleInstanceOnly, Category = "GoldRush")
	bool bHasMissed = false;

	UPROPERTY(VisibleInstanceOnly, Category = "GoldRush")
	int32 MissCount = 0;

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "GoldRush")
	UTextureSwapComponent* TextureSwap;
};
