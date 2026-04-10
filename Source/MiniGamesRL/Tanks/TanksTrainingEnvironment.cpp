// Guchan Alkan - Licensed under GPLv3


#include "Tanks/TanksTrainingEnvironment.h"

#include "TanksPlayer.h"

void UTanksTrainingEnvironment::GatherAgentReward_Implementation(float& OutReward, const int32 AgentId)
{
	ATanksPlayer* Player = Cast<ATanksPlayer>(GetAgent(AgentId));

	if (!IsValid(Player)) return;

	FVector PreviousDistance = FVector::Zero() - Player->GetActorPreviousLocation();
	FVector Distance = FVector::Zero() - Player->GetActorLocation();

	float pd = PreviousDistance.Length();
	float d = Distance.Length();

	// UE_LOG(LogTemp, Display, TEXT("P: %d C: %s"), *PreviousDistance.ToString(), *Distance.ToString());
	// UE_LOG(LogTemp, Display, TEXT("pd: %f d: %f, f: %f"), pd, d, pd - d);

	OutReward = pd - d;
}

void UTanksTrainingEnvironment::GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion,
                                                                     const int32 AgentId)
{
	ATanksPlayer* Player = Cast<ATanksPlayer>(GetAgent(AgentId));

	if (!IsValid(Player)) return;

	OutCompletion = ELearningAgentsCompletion::Running;
}

void UTanksTrainingEnvironment::ResetAgentEpisode_Implementation(const int32 AgentId)
{
	ATanksPlayer* Player = Cast<ATanksPlayer>(GetAgent(AgentId));

	if (!IsValid(Player)) return;

	Player->ResetAgent();
}
