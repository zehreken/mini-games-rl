// Guchan Alkan - Licensed under GPLv3


#include "GoldRush/GoldRushGameMode.h"
#include "GoldRush/GoldRushLearningManager.h"
#include "GoldRush/GoldRushPlayer.h"
#include "Kismet/GameplayStatics.h"

AGoldRushGameMode::AGoldRushGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGoldRushGameMode::BeginPlay()
{
	Super::BeginPlay();

	LearningManager = Cast<AGoldRushLearningManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AGoldRushLearningManager::StaticClass()));
	if (!LearningManager)
		UE_LOG(LogTemp, Error, TEXT("Couldn't find LearningManager"));

	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AGoldRushGameMode::SpawnObstacle,
		1.0f,
		true);

	GetWorldTimerManager().SetTimer(
		CollectibleSpawnTimerHandle,
		this,
		&AGoldRushGameMode::SpawnCollectible,
		5.0f,
		true);
}

void AGoldRushGameMode::SpawnObstacle()
{
	if (!ObstacleClass) return;

	const float RandomY = FMath::RandRange(-1000.0f, 1000.0f);
	const FVector SpawnLocation(0.0f, RandomY, 300.0f);
	const FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation, FVector::OneVector);

	if (AActor* NewObstacle = GetWorld()->SpawnActor<AActor>(ObstacleClass, SpawnTransform))
	{
		if (AGoldRushPlayer* Player = Cast<AGoldRushPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
		{
			Player->Obstacles.Add(NewObstacle);
		}
	}
}

void AGoldRushGameMode::SpawnCollectible()
{
	if (!LearningManager->RunInference && LearningManager->CurriculumManager->GetStepCount() < 1'000'000)
		return;
	
	if (!CollectibleClass) return;

	const float RandomY = FMath::RandRange(-750.0f, 750.0f);
	const FVector SpawnLocation(0.0f, RandomY, -250.0f);
	const FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation, FVector::OneVector);

	if (AActor* NewCollectible = GetWorld()->SpawnActor<AActor>(CollectibleClass, SpawnTransform))
	{
		if (AGoldRushPlayer* Player = Cast<AGoldRushPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
		{
			Player->Collectibles.Add(NewCollectible);
		}
	}
}

void AGoldRushGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (!PlayerClass) return;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FTransform PlayerSpawnTransform(FRotator::ZeroRotator, FVector(0.0f, 0.0f, -300.0f), FVector::OneVector);

	AActor* SpawnedPlayer = GetWorld()->SpawnActor<AActor>(PlayerClass, PlayerSpawnTransform, SpawnParameters);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && SpawnedPlayer)
	{
		if (APawn* PlayerPawn = Cast<APawn>(SpawnedPlayer))
		{
			PC->Possess(PlayerPawn);
		}
	}
}
