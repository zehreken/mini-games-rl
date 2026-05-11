// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RunnersTarget.generated.h"

class ARunnersPlayer; // This is to avoid circular dependency error

UCLASS()
class MINIGAMESRL_API ARunnersTarget : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARunnersTarget();

	UFUNCTION()
	void SetOwnerPlayer(ARunnersPlayer* Player);

	void SetRandomLocation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	FRandomStream RandomStream;

	UPROPERTY()
	ARunnersPlayer* OwnerPlayer;
};
