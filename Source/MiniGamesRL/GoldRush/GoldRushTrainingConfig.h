// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "LearningAgentsPPOTrainer.h"
#include "LearningAgentsTrainer.h"
#include "Engine/DataAsset.h"
#include "GoldRushTrainingConfig.generated.h"

/**
 * 
 */
UCLASS()
class MINIGAMESRL_API UGoldRushTrainingConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Process")
	FLearningAgentsTrainerProcessSettings TrainerProcessSettings;
	
	UPROPERTY(EditAnywhere, Category = "Training")
	FLearningAgentsPPOTrainingSettings PPOTrainingSettings;
};
