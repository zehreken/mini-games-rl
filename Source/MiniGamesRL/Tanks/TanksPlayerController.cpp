// Guchan Alkan - Licensed under GPLv3


#include "TanksPlayerController.h"

void ATanksPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
}
