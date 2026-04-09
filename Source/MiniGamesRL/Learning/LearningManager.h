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
#include "Learning/CurriculumManager.h"
#include "Learning/TrainingConfig.h"
#include "LearningManager.generated.h"

UCLASS()
class MINIGAMESRL_API ALearningManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALearningManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init();

	UPROPERTY(EditAnywhere, Category = "LearningManager")
	bool RunInference = false;

	UPROPERTY(EditAnywhere, Category = "LearningManager")
	bool ReinitialiseNetworks = false;

	UPROPERTY(VisibleInstanceOnly, Category = "LearningManager")
	ULearningAgentsManager* LearningAgentsManager;

	UPROPERTY(VisibleInstanceOnly, Category = "LearningManager")
	UCurriculumManager* CurriculumManager;

	UPROPERTY(EditAnywhere, Category = "LearningManager|Classes")
	TSubclassOf<ULearningAgentsInteractor> InteractorClass;

	UPROPERTY(EditAnywhere, Category = "LearningManager|Classes")
	TSubclassOf<ULearningAgentsTrainingEnvironment> EnvironmentClass;

	UPROPERTY(EditAnywhere, Category = "LearningManager|Classes")
	TSubclassOf<APawn> PlayerClass;

	UPROPERTY(VisibleInstanceOnly, Category = "LearningManager")
	ULearningAgentsInteractor* Interactor;
	
	UPROPERTY(VisibleInstanceOnly, Category = "LearningManager")
	ULearningAgentsPolicy* Policy;

	UPROPERTY(VisibleInstanceOnly, Category = "LearningManager")
	ULearningAgentsCritic* Critic;

	UPROPERTY(VisibleInstanceOnly, Category = "LearningManager")
	ULearningAgentsTrainingEnvironment* TrainingEnv;

	UPROPERTY(VisibleInstanceOnly, Category = "LearningManager")
	ULearningAgentsPPOTrainer* PPOTrainer;

	UPROPERTY(EditAnywhere, Category = "LearningManager|Settings")
	TObjectPtr<UTrainingConfig> TrainerConfig;

	UPROPERTY(EditAnywhere, Category = "LearningManager|Networks")
	TObjectPtr<ULearningAgentsNeuralNetwork> CriticNetwork;

	UPROPERTY(EditAnywhere, Category = "LearningManager|Networks")
	TObjectPtr<ULearningAgentsNeuralNetwork> DecoderNetwork;

	UPROPERTY(EditAnywhere, Category = "LearningManager|Networks")
	TObjectPtr<ULearningAgentsNeuralNetwork> EncoderNetwork;

	UPROPERTY(EditAnywhere, Category = "LearningManager|Networks")
	TObjectPtr<ULearningAgentsNeuralNetwork> PolicyNetwork;
	
	UPROPERTY(EditAnywhere, Category = "LearningManager|Snapshots")
	FFilePath CriticFilePath;

	UPROPERTY(EditAnywhere, Category = "LearningManager|Snapshots")
	FFilePath DecoderFilePath;

	UPROPERTY(EditAnywhere, Category = "LearningManager|Snapshots")
	FFilePath EncoderFilePath;

	UPROPERTY(EditAnywhere, Category = "LearningManager|Snapshots")
	FFilePath PolicyFilePath;

	TUniquePtr<FLearningAgentsTrainerProcess> TrainerProcess;
	TUniquePtr<FLearningAgentsCommunicator> Communicator;
};
