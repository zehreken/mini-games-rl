// Guchan Alkan - Licensed under GPLv3


#include "Tanks/TanksPlayerInteractor.h"

#include "TanksPlayer.h"

void UTanksPlayerInteractor::SpecifyAgentObservation_Implementation(
	FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
	ULearningAgentsObservationSchema* InObservationSchema)
{
	TMap<FName, FLearningAgentsObservationSchemaElement> ObservationSchemaMap;
	ObservationSchemaMap.Add("AlignX", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f));
	ObservationSchemaMap.Add("AlignY", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f));
	OutObservationSchemaElement = ULearningAgentsObservations::SpecifyStructObservation(
		InObservationSchema,
		ObservationSchemaMap);
}

void UTanksPlayerInteractor::GatherAgentObservation_Implementation(
	FLearningAgentsObservationObjectElement& OutObservationObjectElement,
	ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
	ATanksPlayer* Player = Cast<ATanksPlayer>(GetAgent(AgentId));
	if (!IsValid(Player)) return;

	FVector WorldOffset = Player->TargetLocation - Player->GetActorLocation();
	// This is what makes the observation egocentric(from the player's perspective)
	FVector LocalOffset = Player->GetActorTransform().InverseTransformVector(WorldOffset);
	FVector LocalDir = LocalOffset.GetSafeNormal();
	float AlignX = LocalDir.X;
	float AlignY = LocalDir.Y;

	TMap<FName, FLearningAgentsObservationObjectElement> ObservationSchemaMap;
	ObservationSchemaMap.Add("AlignX", ULearningAgentsObservations::MakeFloatObservation(InObservationObject, AlignX));
	ObservationSchemaMap.Add("AlignY", ULearningAgentsObservations::MakeFloatObservation(InObservationObject, AlignY));
	OutObservationObjectElement = ULearningAgentsObservations::MakeStructObservation(
		InObservationObject,
		ObservationSchemaMap);
}

void UTanksPlayerInteractor::SpecifyAgentAction_Implementation(
	FLearningAgentsActionSchemaElement& OutActionSchemaElement, ULearningAgentsActionSchema* InActionSchema)
{
	TMap<FName, FLearningAgentsActionSchemaElement> ActionSchemaMap;
	ActionSchemaMap.Add("LeftThrottle", ULearningAgentsActions::SpecifyFloatAction(InActionSchema, 1.0f));
	ActionSchemaMap.Add("RightThrottle", ULearningAgentsActions::SpecifyFloatAction(InActionSchema, 1.0f));
	OutActionSchemaElement = ULearningAgentsActions::SpecifyStructAction(InActionSchema, ActionSchemaMap);
}

void UTanksPlayerInteractor::PerformAgentAction_Implementation(const ULearningAgentsActionObject* InActionObject,
                                                               const FLearningAgentsActionObjectElement&
                                                               InActionObjectElement, const int32 AgentId)
{
	ATanksPlayer* Player = Cast<ATanksPlayer>(GetAgent(AgentId));
	if (!IsValid(Player)) return;

	TMap<FName, FLearningAgentsActionObjectElement> ActionMap;
	ULearningAgentsActions::GetStructAction(ActionMap, InActionObject, InActionObjectElement);

	const FLearningAgentsActionObjectElement* LeftElem = ActionMap.Find(TEXT("LeftThrottle"));
	const FLearningAgentsActionObjectElement* RightElem = ActionMap.Find(TEXT("RightThrottle"));

	if (!LeftElem || !RightElem) return;

	float LeftThrottle;
	ULearningAgentsActions::GetFloatAction(LeftThrottle, InActionObject, *LeftElem);
	float RightThrottle;
	ULearningAgentsActions::GetFloatAction(RightThrottle, InActionObject, *RightElem);

	Player->SetThrottle(LeftThrottle, RightThrottle);
}
