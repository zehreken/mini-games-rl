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

	TArray<UActorComponent*> ArmActors = GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("Arm"));
	for (UActorComponent* Comp : ArmActors)
	{
		if (UStaticMeshComponent* Arm = Cast<UStaticMeshComponent>(Comp))
		{
			Arms.Add(Arm);
		}
	}

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
		C->SetAngularVelocityDriveTwistAndSwing(true, true);
		C->SetAngularDriveParams(10.0f, 10.0f, 100000.0f);
	}
}

// Called every frame
void ARunnersPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 100.0f;
	DrawDebugDirectionalArrow(GetWorld(), Start, End, 10.0f, FColor::Red, false,
	                          -1.0f, 0.0f, 1.0f);

	// TotalTime += DeltaTime;
	Joint_1->SetAngularVelocityTarget(FVector(0.0f, TotalTime, 0.0f));
	Joint_2->SetAngularVelocityTarget(FVector(0.0f, 0.0f, 0.0f));
	Joint_3->SetAngularVelocityTarget(FVector(0.0f, 0.0f, 0.0f));
	Joint_4->SetAngularVelocityTarget(FVector(0.0f, 0.0f, 0.0f));

	for (UStaticMeshComponent* Arm : Arms)
	{
		Arm->WakeRigidBody();
	}
}

void ARunnersPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LeftForward", IE_Pressed, this, &ARunnersPlayer::LeftForwardOn);
	PlayerInputComponent->BindAction("LeftForward", IE_Released, this, &ARunnersPlayer::LeftForwardOff);
	PlayerInputComponent->BindAction("LeftBack", IE_Pressed, this, &ARunnersPlayer::LeftBackOn);
	PlayerInputComponent->BindAction("LeftBack", IE_Released, this, &ARunnersPlayer::LeftBackOff);

	PlayerInputComponent->BindAction("RightForward", IE_Pressed, this, &ARunnersPlayer::RightForwardOn);
	PlayerInputComponent->BindAction("RightForward", IE_Released, this, &ARunnersPlayer::RightForwardOff);
	PlayerInputComponent->BindAction("RightBack", IE_Pressed, this, &ARunnersPlayer::RightBackOn);
	PlayerInputComponent->BindAction("RightBack", IE_Released, this, &ARunnersPlayer::RightBackOff);
}

void ARunnersPlayer::LeftForwardOn()
{
	TotalTime += 0.5f;
	UE_LOG(LogTemp, Display, TEXT("left forward %f"), TotalTime);
}

void ARunnersPlayer::LeftForwardOff()
{
}

void ARunnersPlayer::LeftBackOn()
{
	TotalTime -= 0.5f;
	UE_LOG(LogTemp, Display, TEXT("left backward %f"), TotalTime);
}

void ARunnersPlayer::LeftBackOff()
{
}

void ARunnersPlayer::RightForwardOn()
{
}

void ARunnersPlayer::RightForwardOff()
{
}

void ARunnersPlayer::RightBackOn()
{
}

void ARunnersPlayer::RightBackOff()
{
}
