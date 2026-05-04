// Guchan Alkan - Licensed under GPLv3


#include "Runners/RunnersPlayerInteractor.h"
#include "Runners/RunnersPlayer.h"

void URunnersPlayerInteractor::SpecifyAgentObservation_Implementation(
	FLearningAgentsObservationSchemaElement& OutObservationSchemaElement,
	ULearningAgentsObservationSchema* InObservationSchema)
{
}

void URunnersPlayerInteractor::GatherAgentObservation_Implementation(
	FLearningAgentsObservationObjectElement& OutObservationObjectElement,
	ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
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
	const FLearningAgentsActionObjectElement& InActionObjectElement, const int32 AgentId)
{
	ARunnersPlayer* Player = Cast<ARunnersPlayer>(GetAgent(AgentId));
	if (!IsValid(Player)) return;

	TMap<FName, FLearningAgentsActionObjectElement> ActionMap;
	ULearningAgentsActions::GetStructAction(ActionMap, InActionObject, InActionObjectElement);

	const FLearningAgentsActionObjectElement* AvtFLElem = ActionMap.Find(TEXT("AVT_FL"));
	const FLearningAgentsActionObjectElement* AvtFRElem = ActionMap.Find(TEXT("AVT_FR"));
	const FLearningAgentsActionObjectElement* AvtBLElem = ActionMap.Find(TEXT("AVT_BL"));
	const FLearningAgentsActionObjectElement* AvtBRElem = ActionMap.Find(TEXT("AVT_BR"));

	if (!AvtFLElem|| !AvtFRElem || !AvtBLElem || !AvtBRElem) return;

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
