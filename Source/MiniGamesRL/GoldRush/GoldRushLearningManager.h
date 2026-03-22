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

	UPROPERTY(visibleAnywhere, Category = "GoldRush")
	ULearningAgentsManager* LearningAgentsManager;

	UPROPERTY(EditAnywhere, Category = "GoldRush|Classes")
	TSubclassOf<ULearningAgentsInteractor> InteractorClass;

	UPROPERTY(VisibleInstanceOnly, Category = "GoldRush")
	ULearningAgentsInteractor* Interactor;
	
	UPROPERTY(VisibleInstanceOnly, Category = "Learning")
	ULearningAgentsPolicy* Policy;

	UPROPERTY(VisibleInstanceOnly, Category = "Learning")
	ULearningAgentsCritic* Critic;

	UPROPERTY(VisibleInstanceOnly, Category = "Learning")
	ULearningAgentsTrainingEnvironment* TrainingEnv;

	UPROPERTY(VisibleInstanceOnly, Category = "Learning")
	ULearningAgentsPPOTrainer* PPOTrainer;

	UPROPERTY(EditAnywhere, Category = "Learning|Networks")
	TObjectPtr<ULearningAgentsNeuralNetwork> CriticNetwork;

	UPROPERTY(EditAnywhere, Category = "Learning|Networks")
	TObjectPtr<ULearningAgentsNeuralNetwork> DecoderNetwork;

	UPROPERTY(EditAnywhere, Category = "Learning|Networks")
	TObjectPtr<ULearningAgentsNeuralNetwork> EncoderNetwork;

	UPROPERTY(EditAnywhere, Category = "Learning|Networks")
	TObjectPtr<ULearningAgentsNeuralNetwork> PolicyNetwork;

	FLearningAgentsTrainerProcess TrainerProcess;
	FLearningAgentsCommunicator Communicator;

};
