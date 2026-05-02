// Guchan Alkan - Licensed under GPLv3


#include "RunnersGameMode.h"

ARunnersGameMode::ARunnersGameMode()
{
}

ALearningManager* ARunnersGameMode::GetLearningManager()
{
	return LearningManager;
}

void ARunnersGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ARunnersGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ARunnersGameMode::OnCheckPhase(int32 StepCount) const
{
}
