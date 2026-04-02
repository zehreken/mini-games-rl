// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LearningAgentsManager.h"
#include "LearningAgentsInteractor.h"
#include "LearningAgentsPolicy.h"
#include "LearningAgentsCritic.h"
#include "LearningAgentsTrainingEnvironment.h"
#include "LearningAgentsPPOTrainer.h"
#include "LearningAgentsCommunicator.h"
#include "GoldRush/GoldRushTrainingConfig.h"
#include "GoldRush/CurriculumManager.h"
#include "GoldRushLearningManager.generated.h"

UCLASS()
class MINIGAMESRL_API AGoldRushLearningManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoldRushLearningManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init();

	UPROPERTY(EditAnywhere, Category = "GoldRush")
	bool RunInference = false;

	UPROPERTY(EditAnywhere, Category = "GoldRush")
	bool ReinitialiseNetworks = false;

	UPROPERTY(VisibleInstanceOnly, Category = "GoldRush")
	ULearningAgentsManager* LearningAgentsManager;

	UPROPERTY(VisibleInstanceOnly, Category = "GoldRush")
	UCurriculumManager* CurriculumManager;

	UPROPERTY(EditAnywhere, Category = "GoldRush|Classes")
	TSubclassOf<ULearningAgentsInteractor> InteractorClass;

	UPROPERTY(VisibleInstanceOnly, Category = "GoldRush")
	ULearningAgentsInteractor* Interactor;
	
	UPROPERTY(VisibleInstanceOnly, Category = "GoldRush")
	ULearningAgentsPolicy* Policy;

	UPROPERTY(VisibleInstanceOnly, Category = "GoldRush")
	ULearningAgentsCritic* Critic;

	UPROPERTY(VisibleInstanceOnly, Category = "GoldRush")
	ULearningAgentsTrainingEnvironment* TrainingEnv;

	UPROPERTY(VisibleInstanceOnly, Category = "GoldRush")
	ULearningAgentsPPOTrainer* PPOTrainer;

	UPROPERTY(EditAnywhere, Category = "GoldRush|Settings")
	TObjectPtr<UGoldRushTrainingConfig> TrainerConfig;

	UPROPERTY(EditAnywhere, Category = "GoldRush|Networks")
	TObjectPtr<ULearningAgentsNeuralNetwork> CriticNetwork;

	UPROPERTY(EditAnywhere, Category = "GoldRush|Networks")
	TObjectPtr<ULearningAgentsNeuralNetwork> DecoderNetwork;

	UPROPERTY(EditAnywhere, Category = "GoldRush|Networks")
	TObjectPtr<ULearningAgentsNeuralNetwork> EncoderNetwork;

	UPROPERTY(EditAnywhere, Category = "GoldRush|Networks")
	TObjectPtr<ULearningAgentsNeuralNetwork> PolicyNetwork;
	
	UPROPERTY(EditAnywhere, Category = "GoldRush|Snapshots")
	FFilePath CriticFilePath;

	UPROPERTY(EditAnywhere, Category = "GoldRush|Snapshots")
	FFilePath DecoderFilePath;

	UPROPERTY(EditAnywhere, Category = "GoldRush|Snapshots")
	FFilePath EncoderFilePath;

	UPROPERTY(EditAnywhere, Category = "GoldRush|Snapshots")
	FFilePath PolicyFilePath;

	TUniquePtr<FLearningAgentsTrainerProcess> TrainerProcess;
	TUniquePtr<FLearningAgentsCommunicator> Communicator;
};
