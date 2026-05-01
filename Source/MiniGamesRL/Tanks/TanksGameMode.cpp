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

void ATanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	TanksPlayer = Cast<ATanksPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ATanksPlayer::StaticClass()));

	if (!IsValid(TanksPlayer)) return;
	UE_LOG(LogTemp, Warning, TEXT("Spawned tanks"));

	LearningManager = Cast<ALearningManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ALearningManager::StaticClass()));
	if (!LearningManager)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't find LearningManager"));
		return;
	}

	for (int32 i = 0; i < 31; i++)
	{
		ATanksPlayer* RedPlayer = GetWorld()->SpawnActor<ATanksPlayer>(TanksPlayerClass,
		                                                               FVector(i / 4 * 400.0f, i % 4 * 400.0f, 400.0f),
		                                                               FRotator::ZeroRotator);
		RedPlayer->bDrivingEnabled = RedPlayer->bDrivingEnabled || LearningManager->RunInference;
		RedPlayer->bShootingEnabled = RedPlayer->bShootingEnabled || LearningManager->RunInference;
		if (i > 14) RedPlayer->SetRedFaction();
		RedPlayers.Add(RedPlayer);
	}

	LearningManager->Init();

	// Enable driving and shooting when running inference
	TanksPlayer->bDrivingEnabled = TanksPlayer->bDrivingEnabled || LearningManager->RunInference;
	TanksPlayer->bShootingEnabled = TanksPlayer->bShootingEnabled || LearningManager->RunInference;

	LearningManager->CurriculumManager->CheckPhaseDelegate.AddUObject(this, &ATanksGameMode::OnCheckPhase);
}

void ATanksGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ATanksGameMode::OnCheckPhase(int32 StepCount) const
{
	if (StepCount > 2'000'000 && !TanksPlayer->bDrivingEnabled)
	{
		TanksPlayer->bDrivingEnabled = true;
		UE_LOG(LogTemp, Display, TEXT("Driving enabled %d %d"), TanksPlayer->bDrivingEnabled, StepCount);
	}
	if (StepCount > 1'000'000 && !TanksPlayer->bShootingEnabled)
	{
		TanksPlayer->bShootingEnabled = true;
		UE_LOG(LogTemp, Display, TEXT("Shooting enabled %d %d"), TanksPlayer->bShootingEnabled, StepCount);
	}
}
