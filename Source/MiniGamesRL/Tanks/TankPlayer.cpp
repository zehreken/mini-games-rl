// Guchan Alkan - Licensed under GPLv3


#include "TankPlayer.h"

#include "Components/BoxComponent.h"


// Sets default values
ATankPlayer::ATankPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Body = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	RootComponent = Body;
	Body->SetBoxExtent(FVector(200.f, 100.f, 50.f)); // half-extents in cm
	Body->SetCollisionProfileName(TEXT("BlockAll"));
	Body->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ATankPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATankPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	constexpr float TrackForce = 500000.0f;
	constexpr float LeftInput = 1.0f;
	constexpr float RightInput = 1.0f;
	constexpr float HalfWidth = 0.5f;
	FVector LeftForce = GetActorForwardVector() * LeftInput * TrackForce;
	FVector RightForce = GetActorForwardVector() * RightInput * TrackForce;
	
	FVector LeftPos = GetActorLocation() + GetActorRightVector() * -HalfWidth;
	FVector RightPos = GetActorLocation() + GetActorRightVector() * HalfWidth;
	
	Body->AddForceAtLocation(LeftForce, LeftPos);
	Body->AddForceAtLocation(RightForce, RightPos);
}

// Called to bind functionality to input
void ATankPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

