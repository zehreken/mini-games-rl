// Guchan Alkan - Licensed under GPLv3

#include "MiniGamesUtils.h"
#include "Engine/World.h"

bool FMiniGamesUtils::GetGroundedLocation(UWorld* World, AActor* IgnoredActor, float X, float Y, FVector& OutLocation)
{
	if (!World) return false;

	constexpr float TraceHalfHeight = 5000.0f;

	FVector Start(X, Y, TraceHalfHeight);
	FVector End(X, Y, -TraceHalfHeight);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	if (IgnoredActor)
	{
		Params.AddIgnoredActor(IgnoredActor);
	}

	bool bHit = World->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, Params);
	if (bHit)
	{
		constexpr float ZOffset = 50.0f;
		OutLocation = HitResult.ImpactPoint + FVector(0.0f, 0.0f, ZOffset);
		return true;
	}

	return false;
}
