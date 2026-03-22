// Guchan Alkan - Licensed under GPLv3


#include "GoldRush/GoldRushLearningManager.h"

#include "GoldRushPlayer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGoldRushLearningManager::AGoldRushLearningManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LearningAgentsManager = CreateDefaultSubobject<ULearningAgentsManager>(TEXT("LearningAgentsManager"));
}

// Called when the game starts or when spawned
void AGoldRushLearningManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoldRushPlayer::StaticClass(), Actors);

	for (AActor* Actor : Actors)
	{
		LearningAgentsManager->AddAgent(Actor);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Added agent"));
	}
}

// Called every frame
void AGoldRushLearningManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
