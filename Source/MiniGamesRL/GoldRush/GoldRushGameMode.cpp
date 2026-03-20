// Fill out your copyright notice in the Description page of Project Settings.


#include "GoldRushGameMode.h"
#include "Kismet/GameplayStatics.h"

AGoldRushGameMode::AGoldRushGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGoldRushGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AGoldRushGameMode::SpawnObstacle,
		3.0f,
		true);
}

void AGoldRushGameMode::SpawnObstacle()
{
	if (!ObstacleClass) return;

	const float RandomY = FMath::RandRange(-750.0f, 750.0f);
	const FVector SpawnLocation(0.0f, RandomY, 300.0f);
	const FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation, FVector::OneVector);

	AActor* NewObstacle = GetWorld()->SpawnActor<AActor>(ObstacleClass, SpawnTransform);

	if (NewObstacle)
		Obstacles.Add(NewObstacle);
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
		if (APawn* PlayerPawn = Cast<APawn>(SpawnedPlayer)	)
		{
			PC->Possess(PlayerPawn);
		}
	}
}
