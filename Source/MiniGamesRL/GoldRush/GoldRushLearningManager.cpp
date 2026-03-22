// Guchan Alkan - Licensed under GPLv3


#include "GoldRush/GoldRushLearningManager.h"

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
	
}

// Called every frame
void AGoldRushLearningManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

