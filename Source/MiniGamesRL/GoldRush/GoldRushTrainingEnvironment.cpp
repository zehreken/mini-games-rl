// Guchan Alkan - Licensed under GPLv3


#include "GoldRush/GoldRushTrainingEnvironment.h"
#include "GoldRush/GoldRushPlayer.h"

void UGoldRushTrainingEnvironment::GatherAgentReward_Implementation(float& OutReward, const int32 AgentId)
{
	AGoldRushPlayer* Player = Cast<AGoldRushPlayer>(GetAgent(AgentId));

	if (!IsValid(Player)) return;

	// Tiny punishment to create urgency
	float Reward = -0.001f;

	if (Player->HasCollected)
	{
		Reward += 1.0f;
	}
	else if (Player->HasMissed)
	{
		Reward += -0.5f;
	}

	Player->HasCollected = false;
	Player->HasMissed = false;

	OutReward = Reward;
}

void UGoldRushTrainingEnvironment::GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion,
                                                                        const int32 AgentId)
{
	AGoldRushPlayer* Player = Cast<AGoldRushPlayer>(GetAgent(AgentId));

	if (!IsValid(Player)) return;

	if (Player->MissCount > 20)
	{
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
