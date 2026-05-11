// Guchan Alkan - Licensed under GPLv3


#include "Runners/RunnersTrainingEnvironment.h"
#include "Runners/RunnersGameMode.h"
#include "Runners/RunnersPlayer.h"
#include "Learning/LearningManager.h"

void URunnersTrainingEnvironment::GatherAgentReward_Implementation(float& OutReward, const int32 AgentId)
{
	ARunnersPlayer* Player = Cast<ARunnersPlayer>(GetAgent(AgentId));

	if (!IsValid(Player)) return;

	float Reward = 0.0f;

	float Alignment = FVector::DotProduct(Player->GetActorUpVector(), FVector::UpVector);
	Reward += FMath::Pow(Alignment, 8.0f);

	if (Player->bHasFlipped) // Alignment < 0.95 => Termination
	{
		Reward -= 1.0f;
	}
	else
	{
		FVector TargetLocation = Player->Target->GetActorLocation();
		FVector ToTargetPrev = TargetLocation - Player->GetActorPreviousLocation();
		FVector ToTarget = TargetLocation - Player->GetActorLocation();

		float DistancePrev = ToTargetPrev.Length();
		float Distance = ToTarget.Length();

		// UE_LOG(LogTemp, Display, TEXT("Test: %f %f"), DistancePrev, Distance);

		FVector WorldOffset = TargetLocation - Player->GetActorLocation();
		// This is what makes the observation egocentric(from the player's perspective)
		FVector LocalOffset = Player->GetActorTransform().InverseTransformVector(WorldOffset);
		FVector LocalDir = LocalOffset.GetSafeNormal();
		float AlignX = LocalDir.X; // 1 if facing directly, -1 if facing the opposite way
		float DistanceDelta = DistancePrev - Distance;
		Reward += DistanceDelta; // Add distance reward
		Reward += AlignX; // Add alignment reward
		if (Player->bHasArrived)
		{
			Reward += 10.0f;
			Player->bHasArrived = false;
		}
	}

	OutReward = Reward;
}

void URunnersTrainingEnvironment::GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion,
                                                                       const int32 AgentId)
{
	ARunnersPlayer* Player = Cast<ARunnersPlayer>(GetAgent(AgentId));

	if (!IsValid(Player)) return;

	if (Player->bHasFlipped)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Agent terminating"));
		OutCompletion = ELearningAgentsCompletion::Termination;
	}
	else
	{
		OutCompletion = ELearningAgentsCompletion::Running;
	}
}

void URunnersTrainingEnvironment::ResetAgentEpisode_Implementation(const int32 AgentId)
{
	ARunnersPlayer* Player = Cast<ARunnersPlayer>(GetAgent(AgentId));

	if (!IsValid(Player)) return;

	if (ARunnersGameMode* RunnersGameMode = Cast<ARunnersGameMode>(GetWorld()->GetAuthGameMode()))
	{
		int32 StepCount = RunnersGameMode->GetLearningManager()->PPOTrainer->GetEpisodeStepNum(AgentId);
		RunnersGameMode->GetLearningManager()->CurriculumManager->EnqueueEpisodeLength(StepCount);
	}

	Player->ResetAgent(AgentId);
}
