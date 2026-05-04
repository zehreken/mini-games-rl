// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "LearningAgentsTrainingEnvironment.h"
#include "RunnersTrainingEnvironment.generated.h"

/**
 * 
 */
UCLASS()
class MINIGAMESRL_API URunnersTrainingEnvironment : public ULearningAgentsTrainingEnvironment
{
	GENERATED_BODY()

public:
	virtual void GatherAgentReward_Implementation(float& OutReward, const int32 AgentId) override;
	virtual void GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion, const int32 AgentId) override;
	virtual void ResetAgentEpisode_Implementation(const int32 AgentId) override;
};
