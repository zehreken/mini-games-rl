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
	Body->SetBoxExtent(FVector(100.0f, 50.0f, 25.0f)); // half-extents in cm
	Body->SetCollisionProfileName(TEXT("BlockAll"));
	Body->SetSimulatePhysics(true);

	LeftInput = 0.0f;
	RightInput = 0.0f;
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

	constexpr float TrackForce = 200'000.0f;
	constexpr float HalfWidth = 25.0f;
	FVector LeftForce = GetActorForwardVector() * LeftInput * TrackForce;
	FVector RightForce = GetActorForwardVector() * RightInput * TrackForce;

	FVector LeftPos = GetActorLocation() + GetActorRightVector() * -HalfWidth;
	FVector RightPos = GetActorLocation() + GetActorRightVector() * HalfWidth;
	
	if (!Body) return;
	UE_LOG(LogTemp, Display, TEXT("left: %f"), LeftInput);

	Body->AddForceAtLocation(LeftForce, LeftPos);
	Body->AddForceAtLocation(RightForce, RightPos);
}

// Called to bind functionality to input
void ATankPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LeftForward", IE_Pressed, this, &ATankPlayer::LeftForwardOn);
	PlayerInputComponent->BindAction("LeftForward", IE_Released, this, &ATankPlayer::LeftForwardOff);
	PlayerInputComponent->BindAction("LeftBack", IE_Pressed, this, &ATankPlayer::LeftBackOn);
	PlayerInputComponent->BindAction("LeftBack", IE_Released, this, &ATankPlayer::LeftBackOff);

	PlayerInputComponent->BindAction("RightForward", IE_Pressed, this, &ATankPlayer::RightForwardOn);
	PlayerInputComponent->BindAction("RightForward", IE_Released, this, &ATankPlayer::RightForwardOff);
	PlayerInputComponent->BindAction("RightBack", IE_Pressed, this, &ATankPlayer::RightBackOn);
	PlayerInputComponent->BindAction("RightBack", IE_Released, this, &ATankPlayer::RightBackOff);
}

void ATankPlayer::LeftForwardOn()
{
	UE_LOG(LogTemp, Display, TEXT("left forward on"));
	LeftInput = 1.f;
}

void ATankPlayer::LeftForwardOff() { LeftInput = 0.f; }
void ATankPlayer::LeftBackOn() { LeftInput = -1.f; }
void ATankPlayer::LeftBackOff() { LeftInput = 0.f; }

void ATankPlayer::RightForwardOn() { RightInput = 1.f; }
void ATankPlayer::RightForwardOff() { RightInput = 0.f; }
void ATankPlayer::RightBackOn() { RightInput = -1.f; }
void ATankPlayer::RightBackOff() { RightInput = 0.f; }
