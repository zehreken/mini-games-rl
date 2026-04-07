// Guchan Alkan - Licensed under GPLv3


#include "GoldRush/GoldRushArenaManager.h"
#include "GoldRush/GoldRushConstants.h"
#include "GoldRush/GoldRushCollectible.h"
#include "GoldRush/GoldRushGameMode.h"
#include "GoldRush/GoldRushObstacle.h"

// Sets default values
AGoldRushArenaManager::AGoldRushArenaManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AGoldRushArenaManager::InitArena(int32 ArenaIndex)
{
	if (AGoldRushGameMode* GoldRushGameMode = Cast<AGoldRushGameMode>(GetWorld()->GetAuthGameMode()))
	{
		constexpr float VerticalSpacing = 700.0f;
		constexpr float HorizontalSpacing = 1600.0f;
		int32 Col = ArenaIndex % 8;
		int32 Row = ArenaIndex / 8;
		ArenaOffset = FVector(0.0f, Col * HorizontalSpacing, Row * VerticalSpacing);
		RandomStream.Initialize(Seed + ArenaIndex);

		GetWorld()->SpawnActor<AActor>(
			GoldRushGameMode->ArenaClass,
			ArenaOffset,
			FRotator::ZeroRotator
		);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		const FTransform PlayerSpawnTransform(FRotator::ZeroRotator, FVector(0.0f, 0.0f, 100.0f) + ArenaOffset,
		                                      FVector::OneVector);

		Player = GetWorld()->SpawnActor<AGoldRushPlayer>(GoldRushGameMode->PlayerClass, PlayerSpawnTransform,
		                                                 SpawnParameters);
		if (Player)
			Player->ArenaOffset = ArenaOffset;
	}
}

// Called when the game starts or when spawned
void AGoldRushArenaManager::BeginPlay()
{
	Super::BeginPlay();

	if (AGoldRushGameMode* GoldRushGameMode = Cast<AGoldRushGameMode>(GetWorld()->GetAuthGameMode()))
	{
		int32 PhaseId = GoldRushGameMode->GetLearningManager()->CurriculumManager->GetCurrentPhaseId();
		float SpawnPeriod = GoldRushConstants::Phases[PhaseId].SpawnPeriod;
		GetWorldTimerManager().SetTimer(
			SpawnTimerHandle,
			this,
			&AGoldRushArenaManager::SpawnObstacle,
			SpawnPeriod,
			true);

		// GetWorldTimerManager().SetTimer(
		//	// 	CollectibleSpawnTimerHandle,
		//	// 	this,
		//	// 	&AGoldRushArenaManager::SpawnCollectible,
		//	// 	5.0f,
		//	// 	true);

		GoldRushGameMode->GetLearningManager()->CurriculumManager->PhaseChangedDelegate.AddUObject(
			this, &AGoldRushArenaManager::OnPhaseChanged);
	}
}

// Called every frame
void AGoldRushArenaManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGoldRushArenaManager::SpawnObstacle()
{
	if (AGoldRushGameMode* GoldRushGameMode = Cast<AGoldRushGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (!GoldRushGameMode->ObstacleClass) return;

		const float RandomY = RandomStream.FRandRange(GoldRushConstants::LeftBorder, GoldRushConstants::RightBorder);
		const FVector SpawnLocation(0.0f, RandomY, 599.0f);
		const FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation + ArenaOffset, FVector::OneVector);

		if (AGoldRushObstacle* NewObstacle = GetWorld()->SpawnActor<AGoldRushObstacle>(
			GoldRushGameMode->ObstacleClass, SpawnTransform))
		{
			NewObstacle->Player = Player;
			Player->Obstacles.Add(NewObstacle);
		}
	}
}

void AGoldRushArenaManager::SpawnCollectible()
{
	if (AGoldRushGameMode* GoldRushGameMode = Cast<AGoldRushGameMode>(GetWorld()->GetAuthGameMode()))
	{
		// if (!GoldRushGameMode->GetLearningManager()->RunInference && GoldRushGameMode->GetLearningManager()->CurriculumManager->GetStepCount() < 1'000'000)
		// 	return;

		if (!GoldRushGameMode->CollectibleClass) return;

		const float RandomY = RandomStream.FRandRange(GoldRushConstants::LeftBorder, GoldRushConstants::RightBorder);
		const FVector SpawnLocation(0.0f, RandomY, 100.0f);
		const FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation + ArenaOffset, FVector::OneVector);

		if (AGoldRushCollectible* NewCollectible = GetWorld()->SpawnActor<AGoldRushCollectible>(
			GoldRushGameMode->CollectibleClass, SpawnTransform))
		{
			NewCollectible->Player = Player;
			Player->Collectibles.Add(NewCollectible);
		}
	}
}

void AGoldRushArenaManager::OnPhaseChanged(int32 PhaseId)
{
	float SpawnPeriod = GoldRushConstants::Phases[PhaseId].SpawnPeriod;
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AGoldRushArenaManager::SpawnObstacle, SpawnPeriod, true);
}
