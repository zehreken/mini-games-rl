// Guchan Alkan - Licensed under GPLv3


#include "Runners/RunnersTrainingEnvironment.h"

void URunnersTrainingEnvironment::GatherAgentReward_Implementation(float& OutReward, const int32 AgentId)
{
	Super::GatherAgentReward_Implementation(OutReward, AgentId);
}

void URunnersTrainingEnvironment::GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion,
	const int32 AgentId)
{
	Super::GatherAgentCompletion_Implementation(OutCompletion, AgentId);
}

void URunnersTrainingEnvironment::ResetAgentEpisode_Implementation(const int32 AgentId)
{
	Super::ResetAgentEpisode_Implementation(AgentId);
}
