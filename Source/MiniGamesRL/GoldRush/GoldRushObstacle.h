// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoldRushObstacle.generated.h"

UCLASS()
class MINIGAMESRL_API AGoldRushObstacle : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGoldRushObstacle();

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
