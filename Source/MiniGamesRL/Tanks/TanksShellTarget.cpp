// Guchan Alkan - Licensed under GPLv3


#include "TanksShellTarget.h"
#include "Tanks/TanksPlayer.h"
#include "Tanks/TanksShell.h"
#include "Kismet/GameplayStatics.h"
#include "Tanks/TanksGameMode.h"
#include "Utils/MiniGamesUtils.h"


// Sets default values
ATanksShellTarget::ATanksShellTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATanksShellTarget::BeginPlay()
{
	Super::BeginPlay();

	RandomStream.Initialize(43);
}

void ATanksShellTarget::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!OtherActor->IsA<ATanksShell>())
		return;

	ATanksPlayer* Player = Cast<ATanksPlayer>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ATanksPlayer::StaticClass()));

	if (!IsValid(Player))
		return;
	
	FVector RandomVector = Player->GetActorLocation() + RandomStream.GetUnitVector() * 1000.0f;
	FVector GroundedLocation;
	FVector NewLocation = FVector(RandomVector.X, RandomVector.Y, 0.0f);
	if (FMiniGamesUtils::GetGroundedLocation(GetWorld(), this, RandomVector.X, RandomVector.Y, GroundedLocation))
	{
		NewLocation = GroundedLocation;
	}

	Player->SetShellHit(OtherActor->GetActorLocation());

	SetActorLocation(NewLocation);
}

// Called every frame
void ATanksShellTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
