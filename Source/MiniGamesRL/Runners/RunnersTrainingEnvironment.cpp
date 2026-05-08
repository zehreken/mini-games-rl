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

	if (Player->bHasFlipped)
	{
		Reward -= 1.0f;
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
		UE_LOG(LogTemp, Warning, TEXT("Agent terminating"));
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
