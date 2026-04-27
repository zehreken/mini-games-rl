// Guchan Alkan - Licensed under GPLv3


#include "TanksTarget.h"
#include "Tanks/TanksPlayer.h"
#include "Utils/MiniGamesUtils.h"


// Sets default values
ATanksTarget::ATanksTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATanksTarget::BeginPlay()
{
	Super::BeginPlay();

	RandomStream.Initialize(42);

	SetRandomLocation();
}

void ATanksTarget::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	ATanksPlayer* Player = Cast<ATanksPlayer>(OtherActor);
	if (!IsValid(Player))
		return;

	Player->bHasArrived = true;

	SetRandomLocation();
}

void ATanksTarget::SetRandomLocation()
{
	FVector RandomVector = RandomStream.GetUnitVector() * 2000.0f;
	FVector GroundedLocation;
	FVector NewLocation = FVector(RandomVector.X, RandomVector.Y, 0.0f);
	if (FMiniGamesUtils::GetGroundedLocation(GetWorld(), this, RandomVector.X, RandomVector.Y, GroundedLocation))
	{
		NewLocation = GroundedLocation;
	}
	
	SetActorLocation(NewLocation);
}

// Called every frame
void ATanksTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
