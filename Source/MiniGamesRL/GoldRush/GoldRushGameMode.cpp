// Guchan Alkan - Licensed under GPLv3


#include "GoldRush/GoldRushGameMode.h"
#include "GoldRush/GoldRushArenaManager.h"
#include "GoldRush/GoldRushLearningManager.h"
#include "Kismet/GameplayStatics.h"

AGoldRushGameMode::AGoldRushGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
}

AGoldRushLearningManager* AGoldRushGameMode::GetLearningManager()
{
	return LearningManager;
}

void AGoldRushGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < 1; i++)
	{
		AGoldRushArenaManager* Arena = GetWorld()->SpawnActor<AGoldRushArenaManager>();
		Arena->InitArena(i);
	}

	LearningManager = Cast<AGoldRushLearningManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGoldRushLearningManager::StaticClass()));
	if (!LearningManager)
		UE_LOG(LogTemp, Error, TEXT("Couldn't find LearningManager"));

	LearningManager->Init();
}

// This is probably unnecessary
void AGoldRushGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (!PlayerClass) return;
}
