// Guchan Alkan - Licensed under GPLv3


#include "Runners/RunnersPlayerController.h"

void ARunnersPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
}