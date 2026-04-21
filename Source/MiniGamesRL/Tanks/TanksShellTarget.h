// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TanksShellTarget.generated.h"

UCLASS()
class MINIGAMESRL_API ATanksShellTarget : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATanksShellTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FRandomStream RandomStream;
};
