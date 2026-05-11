// Guchan Alkan - Licensed under GPLv3


#include "Runners/RunnersTarget.h"
#include "Runners/RunnersPlayer.h"
#include "Utils/MiniGamesUtils.h"

// Sets default values
ARunnersTarget::ARunnersTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ARunnersTarget::SetOwnerPlayer(ARunnersPlayer* Player)
{
	OwnerPlayer = Player;

	SetRandomLocation();
}

// Called when the game starts or when spawned
void ARunnersTarget::BeginPlay()
{
	Super::BeginPlay();

	RandomStream.Initialize(FMath::RandRange(50, 80));
}

void ARunnersTarget::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	ARunnersPlayer* OverlapPlayer = Cast<ARunnersPlayer>(OtherActor);
	if (IsValid(OverlapPlayer) && OwnerPlayer == OverlapPlayer)
	{
		OwnerPlayer->bHasArrived = true;
		SetRandomLocation();
	}
}

void ARunnersTarget::SetRandomLocation()
{
	if (!IsValid(OwnerPlayer))
		return;

	FVector RandomVector = OwnerPlayer->GetActorLocation() + RandomStream.GetUnitVector() * 500.0f;
	FVector GroundedLocation;
	FVector NewLocation = FVector(RandomVector.X, RandomVector.Y, 0.0f);
	if (FMiniGamesUtils::GetGroundedLocation(GetWorld(), this, RandomVector.X, RandomVector.Y, GroundedLocation))
	{
		NewLocation = GroundedLocation;
	}

	SetActorLocation(NewLocation);
}
