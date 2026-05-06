// Guchan Alkan - Licensed under GPLv3


#include "RunnersGameMode.h"
#include "Runners/RunnersPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Learning/LearningManager.h"

ARunnersGameMode::ARunnersGameMode()
{
	PlayerControllerClass = ARunnersPlayerController::StaticClass();
}

ALearningManager* ARunnersGameMode::GetLearningManager()
{
	return LearningManager;
}

void ARunnersGameMode::BeginPlay()
{
	Super::BeginPlay();

	LearningManager = Cast<ALearningManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ALearningManager::StaticClass()));
	if (!LearningManager)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't find LearningManager"));
		return;
	}

	for (int32 i = 1; i < 64; i++)
	{
		ARunnersPlayer* Player = GetWorld()->SpawnActor<ARunnersPlayer>(PlayerClass,
		                                                                FVector(i / 8 * 1200.0f, i % 8 * 1200.0f,
			                                                                400.0f),
		                                                                FRotator::ZeroRotator);
	}

	LearningManager->Init();
	LearningManager->CurriculumManager->CheckPhaseDelegate.AddUObject(this, &ARunnersGameMode::OnCheckPhase);
}

void ARunnersGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ARunnersGameMode::OnCheckPhase(int32 StepCount) const
{
}
