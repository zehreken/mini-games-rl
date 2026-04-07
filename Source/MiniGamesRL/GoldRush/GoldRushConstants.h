// Guchan Alkan - Licensed under GPLv3

#pragma once

struct FPhaseConfig
{
	int32 PhaseThreshold;
	int32 TerminationHitLimit;
	float SpawnPeriod;
	float ObstacleSpeed;
};

namespace GoldRushConstants
{
	constexpr float LeftBorder = -750.0f;
	constexpr float RightBorder = 750.0f;
	constexpr float ArenaWidth = 1500.0f;
	constexpr float ArenaHeight = 650.0f;

	const TArray<FPhaseConfig> Phases = {
		{480, 3, 1.0, 200.0},
		{480, 2, 1.0, 200.0},
		{480, 2, 0.95, 200.0},
		{480, 2, 0.90, 200.0},
		{480, 2, 0.85, 200.0},
		{480, 2, 0.80, 200.0},
		{480, 2, 0.75, 200.0},
		{480, 2, 0.70, 200.0},
		{480, 2, 0.65, 200.0},
		{480, 2, 0.60, 200.0},
		{480, 2, 0.55, 200.0},
		{480, 2, 0.50, 200.0},
		{480, 2, 0.5, 250.0},
		{480, 2, 0.5, 300.0},
		{480, 2, 0.5, 350.0},
		{480, 1, 1.0, 200.0},
	};
};
