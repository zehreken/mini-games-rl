// Guchan Alkan - Licensed under GPLv3


#include "TanksShell.h"

// Sets default values
ATanksShell::ATanksShell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ATanksShell::BeginPlay()
{
	Super::BeginPlay();
}

void ATanksShell::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
}

// Called every frame
void ATanksShell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATanksShell::Launch(FVector Direction, float Speed)
{
	Mesh->AddImpulse(Direction.GetSafeNormal() * Speed, NAME_None, true);
}
