// Guchan Alkan - Licensed under GPLv3


#include "TanksShell.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Tanks/TanksPlayer.h"

// Sets default values
ATanksShell::ATanksShell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetNotifyRigidBodyCollision(true); // "Simulation Generates Hit Events"
	Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ATanksShell::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentHit.AddDynamic(this, &ATanksShell::OnHit);
}

// Called every frame
void ATanksShell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATanksShell::Launch(const FVector& Direction, float Speed, AActor* Tank)
{
	Mesh->IgnoreActorWhenMoving(Tank, true);
	Mesh->AddImpulse(Direction.GetSafeNormal() * Speed, NAME_None, true);
}

void ATanksShell::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit)
{
	ATanksPlayer* Player = Cast<ATanksPlayer>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ATanksPlayer::StaticClass()));

	if (IsValid(Player))
	{
		Player->SetShellHit(Hit.ImpactPoint);
	}

	if (ExplosionEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, Hit.ImpactPoint,
		                                               FRotator::ZeroRotator, FVector(3.0f), true);
	}
	Destroy();
}
