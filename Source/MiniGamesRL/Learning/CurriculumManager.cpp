// Guchan Alkan - Licensed under GPLv3


#include "Learning/CurriculumManager.h"
#include "GoldRush/GoldRushConstants.h"

// Sets default values for this component's properties
UCurriculumManager::UCurriculumManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	StepCount = 0;
	PreviousChangeStepCount = 0;
	CurrentPhaseId = 0;
}

void UCurriculumManager::NextStep()
{
	StepCount += 1;
	if (StepCount % 100 == 0)
		CheckPhase();
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Curriculum manager reporting: %d"), StepCount));
}

void UCurriculumManager::EnqueueEpisodeLength(int32 stepCount)
{
	if (EpisodeLengthBuffer.Num() == 256)
	{
		EpisodeLengthBuffer.RemoveAt(0);
	}

	EpisodeLengthBuffer.Add(stepCount);
}

int32 UCurriculumManager::GetAverageEpisodeLength()
{
	if (EpisodeLengthBuffer.IsEmpty()) return 0;

	int32 Sum = 0;
	for (int32 stepCount : EpisodeLengthBuffer)
	{
		Sum += stepCount;
	}

	return Sum / EpisodeLengthBuffer.Num();
}

int32 UCurriculumManager::GetCurrentPhaseId()
{
	return FMath::Min(CurrentPhaseId, GoldRushConstants::Phases.Num() - 1);
}

// Called when the game starts
void UCurriculumManager::BeginPlay()
{
	Super::BeginPlay();
}

void UCurriculumManager::CheckPhase()
{
	if (GetAverageEpisodeLength() > GoldRushConstants::Phases[CurrentPhaseId].PhaseThreshold && StepCount >
		PreviousChangeStepCount + 4096)
	// I guess this is very very strict since MAX is 512
	{
		CurrentPhaseId += 1;
		PreviousChangeStepCount = StepCount;
		PhaseChangedDelegate.Broadcast(CurrentPhaseId);
		UE_LOG(LogTemp, Log, TEXT("Phase changed: %d TStep: %d"), CurrentPhaseId, StepCount);
	}
}
