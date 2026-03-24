// Guchan Alkan - Licensed under GPLv3


#include "GoldRush/GoldRushCollectible.h"

// Sets default values
AGoldRushCollectible::AGoldRushCollectible()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// The timer still runs, this is better for performance
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AGoldRushCollectible::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&AGoldRushCollectible::DestroyTimerExpired,
		DestroyDelay,
		false);
}

void AGoldRushCollectible::DestroyTimerExpired()
{
	Destroy();
}
