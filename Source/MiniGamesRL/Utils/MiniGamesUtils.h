// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"

class MINIGAMESRL_API FMiniGamesUtils
{
public:
	static bool GetGroundedLocation(UWorld* World, AActor* IgnoredActor, float X, float Y, FVector& OutLocation);
};
