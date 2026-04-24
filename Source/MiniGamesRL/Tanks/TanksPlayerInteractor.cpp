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
	ObservationSchemaMap.Add("ShellCooldown",
	                         ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f));
	ObservationSchemaMap.Add("ShellTargetDirection",
	                         ULearningAgentsObservations::SpecifyDirectionObservation(InObservationSchema));
	ObservationSchemaMap.Add("ShellTargetDistance",
	                         ULearningAgentsObservations::SpecifyFloatObservation(
		                         InObservationSchema, MaxShellTargetDistance));
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

	// Egocentric driving direction
	FVector WorldOffset = Player->TargetLocation - Player->GetActorLocation();
	FVector LocalDir = Player->GetActorTransform().InverseTransformVector(WorldOffset).GetSafeNormal();
	float AlignX = bDrivingEnabled ? LocalDir.X : 0.0f;
	float AlignY = bDrivingEnabled ? LocalDir.Y : 0.0f;

	// Egocentric shell target direction and distance from gun
	FVector WorldDelta = Player->ShellTargetLocation - Player->GunComponent->GetComponentLocation();
	FVector ShellLocalDir = Player->GetActorTransform().InverseTransformVector(WorldDelta).GetSafeNormal();
	float ShellTargetDist = WorldDelta.Length();

	float NormalizedCooldown = Player->GetNormalizedShootTime();

	TMap<FName, FLearningAgentsObservationObjectElement> ObservationMap;
	// Navigation observations
	ObservationMap.Add("AlignX", ULearningAgentsObservations::MakeFloatObservation(InObservationObject, AlignX));
	ObservationMap.Add("AlignY", ULearningAgentsObservations::MakeFloatObservation(InObservationObject, AlignY));
	// Shooting observations
	ObservationMap.Add("ShellCooldown", ULearningAgentsObservations::MakeFloatObservation(
		                   InObservationObject, bShootingEnabled ? NormalizedCooldown : 0.0f));
	ObservationMap.Add("ShellTargetDirection",
	                   ULearningAgentsObservations::MakeDirectionObservation(
		                   InObservationObject, bShootingEnabled ? ShellLocalDir : FVector::ForwardVector));
	ObservationMap.Add("ShellTargetDistance",
	                   ULearningAgentsObservations::MakeFloatObservation(
		                   InObservationObject, bShootingEnabled ? ShellTargetDist : 0.0f));
	OutObservationObjectElement = ULearningAgentsObservations::MakeStructObservation(
		InObservationObject,
		ObservationMap);
}

void UTanksPlayerInteractor::SpecifyAgentAction_Implementation(
	FLearningAgentsActionSchemaElement& OutActionSchemaElement, ULearningAgentsActionSchema* InActionSchema)
{
	TMap<FName, FLearningAgentsActionSchemaElement> ActionSchemaMap;
	ActionSchemaMap.Add("LeftThrottle", ULearningAgentsActions::SpecifyFloatAction(InActionSchema, 1.0f));
	ActionSchemaMap.Add("RightThrottle", ULearningAgentsActions::SpecifyFloatAction(InActionSchema, 1.0f));
	ActionSchemaMap.Add("ShootingDirection", ULearningAgentsActions::SpecifyDirectionAction(InActionSchema));
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
	const FLearningAgentsActionObjectElement* ShootingDirectionElem = ActionMap.Find(TEXT("ShootingDirection"));

	if (!LeftElem || !RightElem || !ShootingDirectionElem) return;

	float LeftThrottle;
	ULearningAgentsActions::GetFloatAction(LeftThrottle, InActionObject, *LeftElem);
	float RightThrottle;
	ULearningAgentsActions::GetFloatAction(RightThrottle, InActionObject, *RightElem);
	FVector ShootingDirection;
	ULearningAgentsActions::GetDirectionAction(ShootingDirection, InActionObject, *ShootingDirectionElem);

	if (bDrivingEnabled)
		Player->SetThrottle(LeftThrottle, RightThrottle);
	if (bShootingEnabled)
		Player->ShootAt(ShootingDirection);
}
