// Guchan Alkan - Licensed under GPLv3


#include "GoldRush/GoldRushLearningManager.h"
#include "GoldRush/GoldRushPlayer.h"
#include "GoldRush/GoldRushPlayerInteractor.h"
#include "GoldRush/GoldRushTrainingEnvironment.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGoldRushLearningManager::AGoldRushLearningManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LearningAgentsManager = CreateDefaultSubobject<ULearningAgentsManager>(TEXT("LearningAgentsManager"));
}

// Called when the game starts or when spawned
void AGoldRushLearningManager::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoldRushPlayer::StaticClass(), Actors);
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	for (AActor* Actor : Actors)
	{
		LearningAgentsManager->AddAgent(Actor);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, LevelName + TEXT(" - Added agent"));

		Actor->AddTickPrerequisiteActor(this);
	}

	Interactor = UGoldRushPlayerInteractor::MakeInteractor(
		LearningAgentsManager,
		InteractorClass,
		TEXT("Interactor")
	);

	// Can convert to RunInference later
	const bool ReinitializeNetwork = true;

	Policy = ULearningAgentsPolicy::MakePolicy(
		LearningAgentsManager,
		Interactor,
		ULearningAgentsPolicy::StaticClass(),
		TEXT("Policy"),
		EncoderNetwork,
		PolicyNetwork,
		DecoderNetwork,
		ReinitializeNetwork,
		ReinitializeNetwork,
		ReinitializeNetwork);

	Critic = ULearningAgentsCritic::MakeCritic(
		LearningAgentsManager,
		Interactor,
		Policy,
		ULearningAgentsCritic::StaticClass(),
		TEXT("Critic"),
		CriticNetwork,
		ReinitializeNetwork);

	TrainingEnv = UGoldRushTrainingEnvironment::MakeTrainingEnvironment(
		LearningAgentsManager,
		UGoldRushTrainingEnvironment::StaticClass(),
		TEXT("TrainingEnvironment"));

	TrainerProcess = MakeUnique<FLearningAgentsTrainerProcess>(ULearningAgentsCommunicatorLibrary::SpawnSharedMemoryTrainingProcess(TrainerConfig->TrainerProcessSettings));
	Communicator = MakeUnique<FLearningAgentsCommunicator>(ULearningAgentsCommunicatorLibrary::MakeSharedMemoryCommunicator(*TrainerProcess));

	PPOTrainer = ULearningAgentsPPOTrainer::MakePPOTrainer(
		LearningAgentsManager,
		Interactor,
		TrainingEnv,
		Policy,
		Critic,
		*Communicator,
		ULearningAgentsPPOTrainer::StaticClass(),
		TEXT("PPOTrainer"));
}

// Called every frame
void AGoldRushLearningManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PPOTrainer->RunTraining();
}
