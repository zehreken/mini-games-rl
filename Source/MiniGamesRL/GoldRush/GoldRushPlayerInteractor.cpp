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
			{"RelativeYToObstacle_1", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f)},
			{"RelativeZToObstacle_1", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f)},
			{"RelativeYToObstacle_2", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f)},
			{"RelativeZToObstacle_2", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f)},
			{"RelativeYToCollectible", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f)},
		});
}

void UGoldRushPlayerInteractor::GatherAgentObservation_Implementation(
	FLearningAgentsObservationObjectElement& OutObservationObjectElement,
	ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
	AGoldRushPlayer* Player = Cast<AGoldRushPlayer>(GetAgent(AgentId));
	if (!IsValid(Player)) return;

	const TArray<FVector> ClosestObstaclesLocation = GetClosestObjectsLocation(Player->Obstacles, Player);
	const FVector RelativeLocation = ClosestObstaclesLocation.Num() > 0
		                                 ? ClosestObstaclesLocation[0] - Player->GetActorLocation()
		                                 : FVector(0.f, 0.f, 599.f);;
	const float RelativeYToObstacle = FMath::Clamp(RelativeLocation.Y / GoldRushConstants::ArenaWidth, -1.0f, 1.0f);
	const float RelativeZToObstacle = FMath::Clamp(RelativeLocation.Z / GoldRushConstants::ArenaHeight, 0.0f, 1.0f);

	const FVector RelativeLocation_1 = ClosestObstaclesLocation.Num() > 1
		                                   ? ClosestObstaclesLocation[1] - Player->GetActorLocation()
		                                   : FVector(0.f, 0.f, 599.f);;
	const float RelativeYToObstacle_1 = FMath::Clamp(RelativeLocation_1.Y / GoldRushConstants::ArenaWidth, -1.0f, 1.0f);
	const float RelativeZToObstacle_1 = FMath::Clamp(RelativeLocation_1.Z / GoldRushConstants::ArenaHeight, 0.0f, 1.0f);

	const FVector RelativeLocation_2 = ClosestObstaclesLocation.Num() > 2
		                                   ? ClosestObstaclesLocation[2] - Player->GetActorLocation()
		                                   : FVector(0.f, 0.f, 599.f);;
	const float RelativeYToObstacle_2 = FMath::Clamp(RelativeLocation_2.Y / GoldRushConstants::ArenaWidth, -1.0f, 1.0f);
	const float RelativeZToObstacle_2 = FMath::Clamp(RelativeLocation_2.Z / GoldRushConstants::ArenaHeight, 0.0f, 1.0f);

	const FVector ClosestCollectibleLocation = GetClosestObjectLocation(Player->Collectibles, Player);
	const FVector CollectibleRelativeLocation = ClosestCollectibleLocation - Player->GetActorLocation();
	const float RelativeYToCollectible = FMath::Clamp(CollectibleRelativeLocation.Y / GoldRushConstants::ArenaWidth,
	                                                  -1.0f, 1.0f);

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
				"RelativeYToObstacle_1",
				ULearningAgentsObservations::MakeFloatObservation(InObservationObject, RelativeYToObstacle_1)
			},
			{
				"RelativeZToObstacle_1",
				ULearningAgentsObservations::MakeFloatObservation(InObservationObject, RelativeZToObstacle_1)
			},
			{
				"RelativeYToObstacle_2",
				ULearningAgentsObservations::MakeFloatObservation(InObservationObject, RelativeYToObstacle_2)
			},
			{
				"RelativeZToObstacle_2",
				ULearningAgentsObservations::MakeFloatObservation(InObservationObject, RelativeZToObstacle_2)
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

TArray<FVector> UGoldRushPlayerInteractor::GetClosestObjectsLocation(const TArray<AActor*>& Objects, AActor* Player)
{
	TArray<TTuple<float, FVector>> DistanceToLocation;

	for (AActor* Object : Objects)
	{
		if (!IsValid(Object)) continue;

		float Distance = Object->GetDistanceTo(Player);
		DistanceToLocation.Add(MakeTuple(Distance, Object->GetActorLocation()));
	}

	DistanceToLocation.Sort([](const TTuple<float, FVector>& A, const TTuple<float, FVector>& B)
	{
		return A.Get<0>() < B.Get<0>();
	});

	TArray<FVector> ClosestObjectsLocation;
	for (const TTuple<float, FVector>& Pair : DistanceToLocation)
	{
		ClosestObjectsLocation.Add(Pair.Get<1>());
	}

	return ClosestObjectsLocation;
}
