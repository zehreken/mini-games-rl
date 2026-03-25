// Guchan Alkan - Licensed under GPLv3


#include "GoldRush/GoldRushPlayer.h"

#include "GoldRushObstacle.h"

// Sets default values
AGoldRushPlayer::AGoldRushPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGoldRushPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AGoldRushPlayer::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->IsA<AGoldRushObstacle>())
	{
		bWasHit = true;

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Hit")));
	}
}

// Called every frame
void AGoldRushPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGoldRushPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGoldRushPlayer::ResetAgent()
{
	SetActorLocation(FVector(0.0f, 0.0f, -300.0f));
	Obstacles.Empty();
	Collectibles.Empty();
	bWasHit = false;
	bHasCollected = false;
	bHasMissed = false;
	MissCount = 0;
}

void AGoldRushPlayer::Move(float Direction)
{
	FVector Location = GetActorLocation();
	float NewPosY = Location.Y + Direction * GetWorld()->GetDeltaSeconds() * 500.0f;
	NewPosY = FMath::Clamp(NewPosY, -750.0f, 750.0f);
	SetActorLocation(FVector(Location.X, NewPosY, Location.Z));
}

void AGoldRushPlayer::MissObject()
{
	bHasMissed = true;
	MissCount += 1;
}
