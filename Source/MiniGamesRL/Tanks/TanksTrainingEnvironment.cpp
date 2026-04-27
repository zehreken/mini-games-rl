// Guchan Alkan - Licensed under GPLv3


#include "Tanks/TanksTrainingEnvironment.h"
#include "TanksPlayer.h"

void UTanksTrainingEnvironment::GatherAgentReward_Implementation(float& OutReward, const int32 AgentId)
{
	ATanksPlayer* Player = Cast<ATanksPlayer>(GetAgent(AgentId));

	if (!IsValid(Player)) return;

	FVector TargetLocation = Player->Target->GetActorLocation();
	FVector PreviousDistance = TargetLocation - Player->GetActorPreviousLocation();
	FVector Distance = TargetLocation - Player->GetActorLocation();

	float pd = PreviousDistance.Length();
	float d = Distance.Length();

	// Temp
	FVector WorldOffset = TargetLocation - Player->GetActorLocation();
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
	// Reward = 0.0f;
	if (Player->bHasArrived)
	{
		Reward += 10.0f;
		Player->bHasArrived = false;
	}

	// if (Player->bShellHit)
	// {
	// 	// float MaxDistance = 2000.0f;
	// 	// float Closeness = FMath::Max(0.0f, 1.0f - Player->ShellHitDelta.Length() / MaxDistance);
	// 	// Reward += 5.0f * Closeness;
	// 	float FalloffRadius = 1000.0f;
	// 	float Closeness = FMath::Exp(-Player->ShellHitDelta.Length() / FalloffRadius);
	// 	Reward += 5.0f * Closeness;
	// 	Player->bShellHit = false;
	// 	UE_LOG(LogTemp, Display, TEXT("shell reward: %f"), 5.0f * Closeness);
	// }

	if (Player->bHasShot) // set this on the tick the cooldown resets and fires
	{
		Player->bHasShot = false;
		float Gravity = FMath::Abs(GetWorld()->GetGravityZ());
		FVector ToTarget = Player->ShellTarget->GetActorLocation() - Player->GunComponent->GetComponentLocation();
		float HorizDist = FVector2D(ToTarget.X, ToTarget.Y).Size();
		float HeightDiff = ToTarget.Z;
		float Speed = 1500.0f;
		float SpeedSq = Speed * Speed;

		float Discriminant = SpeedSq * SpeedSq - Gravity * (Gravity * HorizDist * HorizDist + 2.f * HeightDiff *
			SpeedSq);
		if (Discriminant >= 0.f)
		{
			float Angle = FMath::Atan2(SpeedSq - FMath::Sqrt(Discriminant), Gravity * HorizDist);
			FVector2D Horizontal = FVector2D(ToTarget.X, ToTarget.Y).GetSafeNormal();
			FVector IdealDir = FVector(Horizontal.X * FMath::Cos(Angle),
			                           Horizontal.Y * FMath::Cos(Angle),
			                           FMath::Sin(Angle));
			float Alignment = FVector::DotProduct(Player->LastFiredDirection.GetSafeNormal(), IdealDir);
			Reward += FMath::Pow(Alignment, 20.0f);
		}
		else
		{
			Reward -= 1.0f;
		}
		
		// UE_LOG(LogTemp, Display, TEXT("shell reward: %f"), Reward);
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
