// Guchan Alkan - Licensed under GPLv3


#include "GoldRush/GoldRushTrainingEnvironment.h"
#include "GoldRush/GoldRushPlayer.h"

void UGoldRushTrainingEnvironment::GatherAgentReward_Implementation(float& OutReward, const int32 AgentId)
{
	AGoldRushPlayer* Player = Cast<AGoldRushPlayer>(GetAgent(AgentId));
	
	if (!IsValid(Player)) return;
	
	// Might not be necessary
	constexpr float StepPenalty = -0.01f;
	constexpr float HitReward = -5.0f;
	constexpr float CollectReward = 5.0f;
	constexpr float MissReward = 1.0f;
	float Reward = StepPenalty;

	// The order defines the priority
	if (Player->bWasHit)
	{
		Reward += HitReward;
		
		// GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Hit: %f"), Reward));
	}
	else if (Player->bHasCollected)
	{
		Reward += CollectReward;
		
		// GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Collected: %f"), Reward));
	}
	else if (Player->bHasMissed)
	{
		Reward += MissReward;
		
		// GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Missed: %f"), Reward));
	}

	Player->bWasHit = false;
	Player->bHasCollected = false;
	Player->bHasMissed = false;

	OutReward = Reward;
}

void UGoldRushTrainingEnvironment::GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion,
                                                                        const int32 AgentId)
{
	AGoldRushPlayer* Player = Cast<AGoldRushPlayer>(GetAgent(AgentId));

	if (!IsValid(Player)) return;

	if (Player->HitCount > 0)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Agent %d terminating, HitCount: %d"), AgentId, Player->HitCount);
		OutCompletion = ELearningAgentsCompletion::Termination;
	}
	else
	{
		OutCompletion = ELearningAgentsCompletion::Running;
	}
}

void UGoldRushTrainingEnvironment::ResetAgentEpisode_Implementation(const int32 AgentId)
{
	AGoldRushPlayer* Player = Cast<AGoldRushPlayer>(GetAgent(AgentId));

	if (!IsValid(Player)) return;

	Player->ResetAgent();
}
