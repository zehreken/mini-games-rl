// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LearningAgentsManager.h"
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

};
