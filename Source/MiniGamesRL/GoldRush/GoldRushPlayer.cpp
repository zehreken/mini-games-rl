// Guchan Alkan - Licensed under GPLv3


#include "GoldRush/GoldRushPlayer.h"

#include "GoldRushConstants.h"
#include "GoldRush/GoldRushCollectible.h"
#include "GoldRush/GoldRushObstacle.h"
#include "Components/TextureSwapComponent.h"

// Sets default values
AGoldRushPlayer::AGoldRushPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	TextureSwap = CreateDefaultSubobject<UTextureSwapComponent>(TEXT("TextureSwap"));
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

		// GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Hit")));

		TextureSwap->SwapTexture(2);
	}
	else if (OtherActor->IsA<AGoldRushCollectible>())
	{
		TextureSwap->SwapTexture(3);
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
	for (AActor* Obstacle : Obstacles)
	{
		if (IsValid(Obstacle)) Obstacle->Destroy();
	}
	for (AActor* Collectible : Collectibles)
	{
		if (IsValid(Collectible)) Collectible->Destroy();
	}
	
	SetActorLocation(FVector(0.0f, 0.0f, 100.0f) + ArenaOffset);
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
	NewPosY = FMath::Clamp(NewPosY, GoldRushConstants::LeftBorder + ArenaOffset.Y, GoldRushConstants::RightBorder + ArenaOffset.Y);
	SetActorLocation(FVector(Location.X, NewPosY, Location.Z));

	if (FMath::Abs(Direction) > 0.7f)
	{
		TextureSwap->SwapTexture(1);
	}
}

void AGoldRushPlayer::MissObject()
{
	bHasMissed = true;
	MissCount += 1;
}
