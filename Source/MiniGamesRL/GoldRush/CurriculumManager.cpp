// Guchan Alkan - Licensed under GPLv3


#include "GoldRush/CurriculumManager.h"

// Sets default values for this component's properties
UCurriculumManager::UCurriculumManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UCurriculumManager::OnStep(float Reward)
{
	StepCount += 1;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Curriculum manager reporting: %d"), StepCount));
}


// Called when the game starts
void UCurriculumManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UCurriculumManager::CheckPhase()
{
}
