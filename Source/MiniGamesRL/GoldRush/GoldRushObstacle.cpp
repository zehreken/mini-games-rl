// Guchan Alkan - Licensed under GPLv3


#include "GoldRush/GoldRushObstacle.h"
#include "GoldRush/GoldRushConstants.h"
#include "GoldRush/GoldRushGameMode.h"
#include "GoldRush/GoldRushPlayer.h"

// Sets default values
AGoldRushObstacle::AGoldRushObstacle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGoldRushObstacle::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Player)
	{
		if (!OtherActor->IsA<AGoldRushPlayer>())
			Player->MissObject();
		Player->Obstacles.RemoveSwap(this);
	}

	Destroy();
}

// Called every frame
void AGoldRushObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AGoldRushGameMode* GoldRushGameMode = Cast<AGoldRushGameMode>(GetWorld()->GetAuthGameMode()))
	{
		int32 PhaseId = GoldRushGameMode->GetCurrentPhaseId();
		float Speed = GoldRushConstants::Phases[PhaseId].ObstacleSpeed;
		const FVector Velocity(0.0f, 0.0f, -Speed);
		AddActorLocalOffset(DeltaTime * Velocity);
	}
}
