// Guchan Alkan - Licensed under GPLv3


#include "Runners/RunnersPlayer.h"

// Sets default values
ARunnersPlayer::ARunnersPlayer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TotalTime = 0.0f;
}

// Called when the game starts or when spawned
void ARunnersPlayer::BeginPlay()
{
	Super::BeginPlay();

	TArray<UPhysicsConstraintComponent*> Constraints;
	GetComponents<UPhysicsConstraintComponent>(Constraints);

	for (auto* C : Constraints)
	{
		if (C->GetName() == "Joint1")
		{
			Joint_1 = C;
		}
		else if (C->GetName() == "Joint2")
		{
			Joint_2 = C;
		}
		else if (C->GetName() == "Joint3")
		{
			Joint_3 = C;
		}
		else if (C->GetName() == "Joint4")
		{
			Joint_4 = C;
		}

		C->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
		C->SetAngularVelocityDriveTwistAndSwing(false, false);
		C->SetAngularDriveParams(5000.0f, 200.0f, 100000.0f);
	}
}

// Called every frame
void ARunnersPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TotalTime += DeltaTime;
	// Joint_1->SetAngularVelocityTarget(FVector(TotalTime, 0.0f, 0.0f));
}
