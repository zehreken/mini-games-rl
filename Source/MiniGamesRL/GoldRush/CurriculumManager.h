// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CurriculumManager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPhaseChanged, int32 /*NewPhaseId*/)

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

	int32 GetCurrentPhaseId();

	FOnPhaseChanged PhaseChangedDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void CheckPhase();

	int32 StepCount;

	int32 PreviousChangeStepCount;

	int32 CurrentPhaseId;

	TArray<int32> EpisodeLengthBuffer;
};
