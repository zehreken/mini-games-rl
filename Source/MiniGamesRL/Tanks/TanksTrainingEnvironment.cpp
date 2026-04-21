// Guchan Alkan - Licensed under GPLv3


#include "Tanks/TanksTrainingEnvironment.h"
#include "TanksPlayer.h"

void UTanksTrainingEnvironment::GatherAgentReward_Implementation(float& OutReward, const int32 AgentId)
{
	ATanksPlayer* Player = Cast<ATanksPlayer>(GetAgent(AgentId));

	if (!IsValid(Player)) return;

	FVector PreviousDistance = Player->TargetLocation - Player->GetActorPreviousLocation();
	FVector Distance = Player->TargetLocation - Player->GetActorLocation();

	float pd = PreviousDistance.Length();
	float d = Distance.Length();

	// Temp
	FVector WorldOffset = Player->TargetLocation - Player->GetActorLocation();
	// This is what makes the observation egocentric(from the player's perspective)
	FVector LocalOffset = Player->GetActorTransform().InverseTransformVector(WorldOffset);
	FVector LocalDir = LocalOffset.GetSafeNormal();
	float AlignX = LocalDir.X;
	float AlignY = LocalDir.Y;
	// ====

	// UE_LOG(LogTemp, Display, TEXT("P: %d C: %s"), *PreviousDistance.ToString(), *Distance.ToString());
	// UE_LOG(LogTemp, Display, TEXT("pd: %f d: %f, f: %f"), pd, d, pd - d);

	float DistanceDelta = pd - d;
	float Reward = DistanceDelta + AlignX;
	if (Player->bHitTarget)
	{
		Reward += 10.0f;
		Player->bHitTarget = false;
	}

	if (Player->bShellHitTarget)
	{
		Reward += 10.0f;
		Player->bShellHitTarget = false;
	}
	OutReward = Reward;
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
