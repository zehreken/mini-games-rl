// Guchan Alkan - Licensed under GPLv3


#include "GoldRush/GoldRushPlayerInteractor.h"

#include "GoldRushPlayer.h"

void UGoldRushPlayerInteractor::SpecifyAgentObservation_Implementation(
	FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
	ULearningAgentsObservationSchema* InObservationSchema)
{
	Super::SpecifyAgentObservation_Implementation(OutObservationSchemaElement, InObservationSchema);

	OutObservationSchemaElement = ULearningAgentsObservations::SpecifyStructObservation(
		InObservationSchema,
		{
			{"RelativeYToObstacle", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f)},
			{"RelativeZToObstacle", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f)},
		});
}

void UGoldRushPlayerInteractor::GatherAgentObservation_Implementation(
	FLearningAgentsObservationObjectElement& OutObservationObjectElement,
	ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
	Super::GatherAgentObservation_Implementation(OutObservationObjectElement, InObservationObject, AgentId);

	AGoldRushPlayer* Player = Cast<AGoldRushPlayer>(GetAgent(AgentId));
	if (!IsValid(Player)) return;

	const FVector ClosestObstacleLocation = GetClosestObstacleLocation(Player->Obstacles, Player);

	const float RelativeYToObstacle = FMath::Clamp(ClosestObstacleLocation.Y / 800.0f, -1.0f, 1.0f);
	const float RelativeZToObstacle = FMath::Clamp(ClosestObstacleLocation.Z / 650.0f, 0.0f, 1.0f);

	OutObservationObjectElement = ULearningAgentsObservations::MakeStructObservation(
		InObservationObject,
		{
			{
				"RelativeYToObstacle",
				ULearningAgentsObservations::MakeFloatObservation(InObservationObject, RelativeYToObstacle)
			},
			{
				"RelativeZToObstacle",
				ULearningAgentsObservations::MakeFloatObservation(InObservationObject, RelativeZToObstacle)
			}
		});
}

void UGoldRushPlayerInteractor::SpecifyAgentAction_Implementation(
	FLearningAgentsActionSchemaElement& OutActionSchemaElement, ULearningAgentsActionSchema* InActionSchema)
{
	Super::SpecifyAgentAction_Implementation(OutActionSchemaElement, InActionSchema);

	OutActionSchemaElement = ULearningAgentsActions::SpecifyFloatAction(InActionSchema, 1.0f, "Direction");
}

void UGoldRushPlayerInteractor::PerformAgentAction_Implementation(
	const ULearningAgentsActionObject* InActionObject, const FLearningAgentsActionObjectElement& InActionObjectElement,
	const int32 AgentId)
{
	Super::PerformAgentAction_Implementation(InActionObject, InActionObjectElement, AgentId);

	AGoldRushPlayer* Player = Cast<AGoldRushPlayer>(GetAgent(AgentId));
	if (!IsValid(Player)) return;

	float Direction;
	ULearningAgentsActions::GetFloatAction(Direction, InActionObject, InActionObjectElement, "Direction");

	Player->Move(Direction);
}

FVector UGoldRushPlayerInteractor::GetClosestObstacleLocation(const TArray<AActor*>& Obstacles, AActor* Player)
{
	float ClosestDistance = TNumericLimits<float>::Max();
	FVector ClosestLocation = FVector::ZeroVector;

	for (AActor* Obstacle : Obstacles)
	{
		if (!IsValid(Obstacle)) continue;

		float Distance = Obstacle->GetDistanceTo(Player);
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestLocation = Obstacle->GetActorLocation();
		}
	}

	return ClosestLocation;
}
