// Guchan Alkan - Licensed under GPLv3


#include "GoldRush/GoldRushPlayerInteractor.h"
#include "GoldRush/GoldRushConstants.h"
#include "GoldRush/GoldRushPlayer.h"

static const TArray<FName> AgentObservationNames = {
	"RelativeYToObstacle",
	"RelativeZToObstacle",
	"RelativeYToObstacle_1",
	"RelativeZToObstacle_1",
	"RelativeYToObstacle_2",
	"RelativeZToObstacle_2",
	"RelativeYToObstacle_3",
	"RelativeZToObstacle_3",
	"RelativeYToObstacle_4",
	"RelativeZToObstacle_4",
	"RelativeYToObstacle_5",
	"RelativeZToObstacle_5",
	"RelativeYToCollectible",
	"AgentLocalY",
};

void UGoldRushPlayerInteractor::SpecifyAgentObservation_Implementation(
	FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
	ULearningAgentsObservationSchema* InObservationSchema)
{
	TMap<FName, FLearningAgentsObservationSchemaElement> ObservationSchemaMap;
	for (FName name : AgentObservationNames)
	{
		ObservationSchemaMap.Add(name, ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f));
	}
	OutObservationSchemaElement = ULearningAgentsObservations::SpecifyStructObservation(
		InObservationSchema,
		ObservationSchemaMap);
}

void UGoldRushPlayerInteractor::GatherAgentObservation_Implementation(
	FLearningAgentsObservationObjectElement& OutObservationObjectElement,
	ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
	AGoldRushPlayer* Player = Cast<AGoldRushPlayer>(GetAgent(AgentId));
	if (!IsValid(Player)) return;

	TArray<float> ObservationValues;
	const TArray<FVector> ClosestObstaclesLocation = GetClosestObjectsLocation(Player->Obstacles, Player);
	for (int32 i = 0; i < 6; i++) // 6 is hardcoded because we have 6 slots for observations
	{
		if (ClosestObstaclesLocation.Num() > i)
		{
			const FVector RelativeLocation = ClosestObstaclesLocation[i] - Player->GetActorLocation();
			const float RelativeYToObstacle = FMath::Clamp(RelativeLocation.Y / GoldRushConstants::ArenaWidth, -1.0f,
			                                               1.0f);
			const float RelativeZToObstacle = FMath::Clamp(RelativeLocation.Z / GoldRushConstants::ArenaHeight, 0.0f,
			                                               1.0f);
			ObservationValues.Add(RelativeYToObstacle);
			ObservationValues.Add(RelativeZToObstacle);
		}
		else
		{
			constexpr float NoObstacleSentinel = 2.0f;
			ObservationValues.Add(NoObstacleSentinel);
			ObservationValues.Add(NoObstacleSentinel);
		}
	}

	const FVector ClosestCollectibleLocation = GetClosestObjectLocation(Player->Collectibles, Player);
	const FVector CollectibleRelativeLocation = ClosestCollectibleLocation - Player->GetActorLocation();
	const float RelativeYToCollectible = FMath::Clamp(CollectibleRelativeLocation.Y / GoldRushConstants::ArenaWidth,
	                                                  -1.0f, 1.0f);
	ObservationValues.Add(RelativeYToCollectible);

	const FVector AgentLocal = Player->GetActorLocation() - Player->ArenaOffset;
	const float NormalizedAgentLocalY = FMath::Clamp(AgentLocal.Y / (GoldRushConstants::RightBorder - 100.f), -1.0f,
	                                                 1.0f);
	ObservationValues.Add(NormalizedAgentLocalY);


	TMap<FName, FLearningAgentsObservationObjectElement> ObservationSchemaMap;
	for (int32 i = 0; i < AgentObservationNames.Num(); i++)
	{
		ObservationSchemaMap.Add(AgentObservationNames[i],
		                         ULearningAgentsObservations::MakeFloatObservation(
			                         InObservationObject, ObservationValues[i]));
	}


	OutObservationObjectElement = ULearningAgentsObservations::MakeStructObservation(
		InObservationObject,
		ObservationSchemaMap);
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

FVector UGoldRushPlayerInteractor::GetClosestObjectLocation(const TArray<AActor*>& Objects, const AActor* Player)
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

TArray<FVector> UGoldRushPlayerInteractor::GetClosestObjectsLocation(const TArray<AActor*>& Objects,
                                                                     const AActor* Player)
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
