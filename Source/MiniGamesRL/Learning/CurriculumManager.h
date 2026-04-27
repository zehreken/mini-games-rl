// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CurriculumManager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCheckPhase, int32 /*StepCount*/)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINIGAMESRL_API UCurriculumManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCurriculumManager();

	void NextStep();

	FORCEINLINE int32 GetStepCount() const { return StepCount; }

	void EnqueueEpisodeLength(int32 stepCount);

	int32 GetAverageEpisodeLength();

	FOnCheckPhase CheckPhaseDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	int32 StepCount;

	TArray<int32> EpisodeLengthBuffer;
};
