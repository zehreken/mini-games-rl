// Guchan Alkan - Licensed under GPLv3


#include "RunnersGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Learning/LearningManager.h"

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

	LearningManager = Cast<ALearningManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ALearningManager::StaticClass()));
	if (!LearningManager)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't find LearningManager"));
		return;
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
