// Guchan Alkan - Licensed under GPLv3


#include "Runners/RunnersPlayerInteractor.h"
#include "Runners/RunnersPlayer.h"

void URunnersPlayerInteractor::SpecifyAgentObservation_Implementation(
	FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
	ULearningAgentsObservationSchema* InObservationSchema)
{
	TMap<FName, FLearningAgentsObservationSchemaElement> ObservationSchemaMap;
	ObservationSchemaMap.Add("LegAngleFL",
	                         ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f));
	ObservationSchemaMap.Add("LegAngleFR",
	                         ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f));
	ObservationSchemaMap.Add("LegAngleBL",
	                         ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f));
	ObservationSchemaMap.Add("LegAngleBR",
	                         ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f));
	ObservationSchemaMap.Add("LegVelocityFL",
	                         ULearningAgentsObservations::SpecifyVelocityObservation(InObservationSchema, 50.0f));
	ObservationSchemaMap.Add("LegVelocityFR",
	                         ULearningAgentsObservations::SpecifyVelocityObservation(InObservationSchema, 50.0f));
	ObservationSchemaMap.Add("LegVelocityBL",
	                         ULearningAgentsObservations::SpecifyVelocityObservation(InObservationSchema, 50.0f));
	ObservationSchemaMap.Add("LegVelocityBR",
	                         ULearningAgentsObservations::SpecifyVelocityObservation(InObservationSchema, 50.0f));
	ObservationSchemaMap.
		Add("LookAtX", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f));
	ObservationSchemaMap.
		Add("LookAtY", ULearningAgentsObservations::SpecifyFloatObservation(InObservationSchema, 1.0f));
	OutObservationSchemaElement = ULearningAgentsObservations::SpecifyStructObservation(
		InObservationSchema,
		ObservationSchemaMap);
}

void URunnersPlayerInteractor::GatherAgentObservation_Implementation(
	FLearningAgentsObservationObjectElement& OutObservationObjectElement,
	ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
	ARunnersPlayer* Player = Cast<ARunnersPlayer>(GetAgent(AgentId));
	if (!IsValid(Player)) return;

	TMap<FName, FLearningAgentsObservationObjectElement> ObservationMap;

	ObservationMap.Add("LegAngleFL",
	                   ULearningAgentsObservations::MakeFloatObservation(
		                   InObservationObject, Player->GetJointAngleFL()));
	ObservationMap.Add("LegAngleFR",
	                   ULearningAgentsObservations::MakeFloatObservation(
		                   InObservationObject, Player->GetJointAngleFR()));
	ObservationMap.Add("LegAngleBL",
	                   ULearningAgentsObservations::MakeFloatObservation(
		                   InObservationObject, Player->GetJointAngleBL()));
	ObservationMap.Add("LegAngleBR",
	                   ULearningAgentsObservations::MakeFloatObservation(
		                   InObservationObject, Player->GetJointAngleBR()));

	ObservationMap.Add("LegVelocityFL",
	                   ULearningAgentsObservations::MakeVelocityObservation(
		                   InObservationObject, Player->GetAngularVelocityFL()));
	ObservationMap.Add("LegVelocityFR",
	                   ULearningAgentsObservations::MakeVelocityObservation(
		                   InObservationObject, Player->GetAngularVelocityFR()));
	ObservationMap.Add("LegVelocityBL",
	                   ULearningAgentsObservations::MakeVelocityObservation(
		                   InObservationObject, Player->GetAngularVelocityBL()));
	ObservationMap.Add("LegVelocityBR",
	                   ULearningAgentsObservations::MakeVelocityObservation(
		                   InObservationObject, Player->GetAngularVelocityBR()));

	FVector LocalDir = Player->GetActorTransform().InverseTransformVector(Player->LookAtDirection).GetSafeNormal();
	float LookAtX = Player->bLookingEnabled ? LocalDir.X : 0.0f;
	float LookAtY = Player->bLookingEnabled ? LocalDir.Y : 0.0f;
	ObservationMap.Add("LookAtX", ULearningAgentsObservations::MakeFloatObservation(
		                   InObservationObject, LookAtX));
	ObservationMap.Add("LookAtY", ULearningAgentsObservations::MakeFloatObservation(
		                   InObservationObject, LookAtY));
	OutObservationObjectElement = ULearningAgentsObservations::MakeStructObservation(
		InObservationObject,
		ObservationMap);
}

void URunnersPlayerInteractor::SpecifyAgentAction_Implementation(
	FLearningAgentsActionSchemaElement& OutActionSchemaElement, ULearningAgentsActionSchema* InActionSchema)
{
	// AVT: Angular velocity target
	TMap<FName, FLearningAgentsActionSchemaElement> ActionSchemaMap;
	ActionSchemaMap.Add("AVT_FL", ULearningAgentsActions::SpecifyFloatAction(InActionSchema, 1.0f));
	ActionSchemaMap.Add("AVT_FR", ULearningAgentsActions::SpecifyFloatAction(InActionSchema, 1.0f));
	ActionSchemaMap.Add("AVT_BL", ULearningAgentsActions::SpecifyFloatAction(InActionSchema, 1.0f));
	ActionSchemaMap.Add("AVT_BR", ULearningAgentsActions::SpecifyFloatAction(InActionSchema, 1.0f));
	OutActionSchemaElement = ULearningAgentsActions::SpecifyStructAction(InActionSchema, ActionSchemaMap);
}

void URunnersPlayerInteractor::PerformAgentAction_Implementation(const ULearningAgentsActionObject* InActionObject,
                                                                 const FLearningAgentsActionObjectElement&
                                                                 InActionObjectElement, const int32 AgentId)
{
	ARunnersPlayer* Player = Cast<ARunnersPlayer>(GetAgent(AgentId));
	if (!IsValid(Player)) return;

	TMap<FName, FLearningAgentsActionObjectElement> ActionMap;
	ULearningAgentsActions::GetStructAction(ActionMap, InActionObject, InActionObjectElement);

	const FLearningAgentsActionObjectElement* AvtFLElem = ActionMap.Find(TEXT("AVT_FL"));
	const FLearningAgentsActionObjectElement* AvtFRElem = ActionMap.Find(TEXT("AVT_FR"));
	const FLearningAgentsActionObjectElement* AvtBLElem = ActionMap.Find(TEXT("AVT_BL"));
	const FLearningAgentsActionObjectElement* AvtBRElem = ActionMap.Find(TEXT("AVT_BR"));

	if (!AvtFLElem || !AvtFRElem || !AvtBLElem || !AvtBRElem) return;

	float AvtFL;
	ULearningAgentsActions::GetFloatAction(AvtFL, InActionObject, *AvtFLElem);
	float AvtFR;
	ULearningAgentsActions::GetFloatAction(AvtFR, InActionObject, *AvtFRElem);
	float AvtBL;
	ULearningAgentsActions::GetFloatAction(AvtBL, InActionObject, *AvtBLElem);
	float AvtBR;
	ULearningAgentsActions::GetFloatAction(AvtBR, InActionObject, *AvtBRElem);

	Player->SetVelocityTargetFL(AvtFL);
	Player->SetVelocityTargetFR(AvtFR);
	Player->SetVelocityTargetBL(AvtBL);
	Player->SetVelocityTargetBR(AvtBR);
}
