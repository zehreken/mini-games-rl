// Guchan Alkan - Licensed under GPLv3


#include "GoldRush/GoldRushPlayerInteractor.h"
#include "GoldRush/GoldRushConstants.h"
#include "GoldRush/GoldRushPlayer.h"

void UGoldRushPlayerInteractor::SpecifyAgentObservation_Implementation(
	FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
	ULearningAgentsObservationSchema* InObservationSchema)
{
	OutObservationSchemaElement = ULearningAgentsObservations::SpecifyStructObservation(
		InObservationSchema,
		{
			{"RelativeYToObstacle", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f)},
			{"RelativeZToObstacle", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f)},
			{"RelativeYToCollectible", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f)},
		});
}

void UGoldRushPlayerInteractor::GatherAgentObservation_Implementation(
	FLearningAgentsObservationObjectElement& OutObservationObjectElement,
	ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
	AGoldRushPlayer* Player = Cast<AGoldRushPlayer>(GetAgent(AgentId));
	if (!IsValid(Player)) return;

	const FVector ClosestObstacleLocation = GetClosestObjectLocation(Player->Obstacles, Player);
	const FVector RelativeLocation = ClosestObstacleLocation - Player->GetActorLocation();
	const float RelativeYToObstacle = FMath::Clamp(RelativeLocation.Y / GoldRushConstants::ArenaWidth, -1.0f, 1.0f);
	const float RelativeZToObstacle = FMath::Clamp(RelativeLocation.Z / GoldRushConstants::ArenaHeight, 0.0f, 1.0f);

	const FVector ClosestCollectibleLocation = GetClosestObjectLocation(Player->Collectibles, Player);
	const FVector CollectibleRelativeLocation = ClosestCollectibleLocation - Player->GetActorLocation();
	const float RelativeYToCollectible = FMath::Clamp(CollectibleRelativeLocation.Y / GoldRushConstants::ArenaWidth, -1.0f, 1.0f);

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
			},
			{
				"RelativeYToCollectible",
				ULearningAgentsObservations::MakeFloatObservation(InObservationObject, RelativeYToCollectible)
			},
		});
}

void UGoldRushPlayerInteractor::SpecifyAgentAction_Implementation(
	FLearningAgentsActionSchemaElement& OutActionSchemaElement, ULearningAgentsActionSchema* InActionSchema)
{
	OutActionSchemaElement = ULearningAgentsActions::SpecifyFloatAction(InActionSchema, 1.0f, "Direction");
}

void UGoldRushPlayerInteractor::PerformAgentAction_Implementation(
	const ULearningAgentsActionObject* InActionObject, const FLearningAgentsActionObjectElement& InActionObjectElement,
	const int32 AgentId)
{
	AGoldRushPlayer* Player = Cast<AGoldRushPlayer>(GetAgent(AgentId));
	if (!IsValid(Player)) return;

	float Direction;
	ULearningAgentsActions::GetFloatAction(Direction, InActionObject, InActionObjectElement, "Direction");

	Player->Move(Direction);
}

FVector UGoldRushPlayerInteractor::GetClosestObjectLocation(const TArray<AActor*>& Objects, AActor* Player)
{
	float ClosestDistance = TNumericLimits<float>::Max();
	FVector ClosestLocation = Player->GetActorLocation();

	for (AActor* Object : Objects)
	{
		if (!IsValid(Object)) continue;

		float Distance = Object->GetDistanceTo(Player);
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestLocation = Object->GetActorLocation();
		}
	}

	return ClosestLocation;
}
