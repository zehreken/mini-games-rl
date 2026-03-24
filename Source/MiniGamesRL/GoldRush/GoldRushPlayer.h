// Guchan Alkan - Licensed under GPLv3


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoldRushPlayer.generated.h"

UCLASS()
class MINIGAMESRL_API AGoldRushPlayer : public APawn
{
	GENERATED_BODY()

public:
	AGoldRushPlayer();

	UFUNCTION(BlueprintCallable)
	void ResetAgent();

	UFUNCTION(BlueprintCallable)
	void Move(float Direction);
	
	UFUNCTION(BlueprintCallable)
	void MissObject();

	UPROPERTY(BlueprintReadWrite, Category = "GoldRush")
	TArray<AActor*> Obstacles;

	// True when player is hit by an obstacle
	UPROPERTY(BlueprintReadWrite, Category = "GoldRush")
	bool WasHit = false;

	// True when player collects a collectible
	UPROPERTY(BlueprintReadWrite, Category = "GoldRush")
	bool HasCollected = false;

	// True when an obstacle is avoided
	UPROPERTY(BlueprintReadWrite, Category = "GoldRush")
	bool HasMissed = false;

	UPROPERTY(BlueprintReadWrite, Category = "GoldRush")
	int32 MissCount = 0;

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};