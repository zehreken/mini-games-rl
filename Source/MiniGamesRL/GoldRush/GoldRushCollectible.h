// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoldRushCollectible.generated.h"

UCLASS()
class MINIGAMESRL_API AGoldRushCollectible : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGoldRushCollectible();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FTimerHandle DestroyTimerHandle;

	UFUNCTION()
	void DestroyTimerExpired();

	UPROPERTY(EditAnywhere, Category="Collectible")
	float DestroyDelay = 3.0f;
};
