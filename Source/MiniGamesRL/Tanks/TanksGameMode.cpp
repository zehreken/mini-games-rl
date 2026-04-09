// Guchan Alkan - Licensed under GPLv3


#include "Tanks/TanksGameMode.h"
#include "Tanks/TanksPlayerController.h"

ATanksGameMode::ATanksGameMode()
{
	PlayerControllerClass = ATanksPlayerController::StaticClass();
}
