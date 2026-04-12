// Guchan Alkan - Licensed under GPLv3


#include "TanksTarget.h"

#include "TanksGameMode.h"


// Sets default values
ATanksTarget::ATanksTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATanksTarget::BeginPlay()
{
	Super::BeginPlay();

	RandomStream.Initialize(42);
}

void ATanksTarget::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	ATanksGameMode* GameMode = Cast<ATanksGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode)) return;

	FVector RandomVector = RandomStream.GetUnitVector() * 2000.0f;
	FVector GroundedLocation;
	FVector NewLocation = FVector(RandomVector.X, RandomVector.Y, 0.0f);
	if (GetGroundedLocation(RandomVector.X, RandomVector.Y, GroundedLocation))
	{
		NewLocation = GroundedLocation;
	}
	SetActorLocation(NewLocation);
	GameMode->SetTargetLocation(NewLocation);
}

// Called every frame
void ATanksTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ATanksTarget::GetGroundedLocation(const float X, const float Y, FVector& OutLocation) const
{
	constexpr float TraceHalfHeight = 5000.0f;

	FVector Start(X, Y, TraceHalfHeight);
	FVector End(X, Y, -TraceHalfHeight);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult, Start, End, ECC_WorldStatic, Params
	);

	if (bHit)
	{
		constexpr float ZOffset = 50.0f;
		OutLocation = HitResult.ImpactPoint + FVector(0.0f, 0.0f, ZOffset);
		return true;
	}

	return false;
}
