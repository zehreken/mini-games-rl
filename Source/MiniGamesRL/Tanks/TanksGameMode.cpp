// Guchan Alkan - Licensed under GPLv3


#include "Tanks/TanksGameMode.h"
#include "Tanks/TanksPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Tanks/TanksPlayerController.h"

ATanksGameMode::ATanksGameMode()
{
	PlayerControllerClass = ATanksPlayerController::StaticClass();
}

ALearningManager* ATanksGameMode::GetLearningManager()
{
	return LearningManager;
}

void ATanksGameMode::SetTargetLocation(FVector Location)
{
	ATanksPlayer* Player = Cast<ATanksPlayer>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ATanksPlayer::StaticClass()));

	if (!IsValid(Player)) return;

	Player->SetTargetLocation(Location);
}

void ATanksGameMode::SetShellTargetLocation(FVector Location)
{
	ATanksPlayer* Player = Cast<ATanksPlayer>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ATanksPlayer::StaticClass()));

	if (!IsValid(Player)) return;

	Player->SetShellTargetLocation(Location);
}


void ATanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	LearningManager = Cast<ALearningManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ALearningManager::StaticClass()));
	if (!LearningManager)
		UE_LOG(LogTemp, Error, TEXT("Couldn't find LearningManager"));

	LearningManager->Init();
}

void ATanksGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}
