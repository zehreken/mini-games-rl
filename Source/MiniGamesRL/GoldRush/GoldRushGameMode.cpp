// Guchan Alkan - Licensed under GPLv3


#include "GoldRush/GoldRushGameMode.h"
#include "GoldRush/GoldRushArenaManager.h"
#include "GoldRush/GoldRushConstants.h"
#include "GoldRush/GoldRushLearningManager.h"
#include "GoldRush/GoldRushPlayerController.h"
#include "Kismet/GameplayStatics.h"

AGoldRushGameMode::AGoldRushGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	PlayerControllerClass = AGoldRushPlayerController::StaticClass();

	PreviousChangeStepCount = 0;
	CurrentPhaseId = 0;
}

AGoldRushLearningManager* AGoldRushGameMode::GetLearningManager()
{
	return LearningManager;
}

int32 AGoldRushGameMode::GetCurrentPhaseId() const
{
	return CurrentPhaseId;
}

void AGoldRushGameMode::BeginPlay()
{
	Super::BeginPlay();

	LearningManager = Cast<AGoldRushLearningManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGoldRushLearningManager::StaticClass()));
	if (!LearningManager)
		UE_LOG(LogTemp, Error, TEXT("Couldn't find LearningManager"));

	for (int32 i = 0; i < 64; i++)
	{
		AGoldRushArenaManager* Arena = GetWorld()->SpawnActor<AGoldRushArenaManager>();
		ArenaManagers.Add(Arena);
		Arena->InitArena(i);
	}

	LearningManager->Init();

	GetLearningManager()->CurriculumManager->CheckPhaseDelegate.AddUObject(
		this, &AGoldRushGameMode::OnCheckPhase);
}

// This is probably unnecessary
void AGoldRushGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (!PlayerClass) return;
}

void AGoldRushGameMode::OnCheckPhase(int32 StepCount)
{
	if (AGoldRushGameMode* GoldRushGameMode = Cast<AGoldRushGameMode>(GetWorld()->GetAuthGameMode()))
	{
		UCurriculumManager* CurriculumManager = GoldRushGameMode->GetLearningManager()->CurriculumManager;
		{
			if (CurriculumManager->GetAverageEpisodeLength() > GoldRushConstants::Phases[CurrentPhaseId].PhaseThreshold
				&& StepCount >
				PreviousChangeStepCount + 4096)
			// I guess this is very strict since MAX is 512
			{
				CurrentPhaseId += 1;
				PreviousChangeStepCount = StepCount;
				UE_LOG(LogTemp, Log, TEXT("Phase changed: %d TStep: %d"), CurrentPhaseId, StepCount);

				float SpawnPeriod = GoldRushConstants::Phases[CurrentPhaseId].SpawnPeriod;
				for (AGoldRushArenaManager* ArenaManager : ArenaManagers)
				{
					ArenaManager->SetTimer(SpawnPeriod);
				}
			}
		}
	}
}
