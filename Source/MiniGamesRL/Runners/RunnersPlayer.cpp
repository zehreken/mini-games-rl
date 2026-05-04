// Guchan Alkan - Licensed under GPLv3


#include "Runners/RunnersPlayer.h"

// Sets default values
ARunnersPlayer::ARunnersPlayer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARunnersPlayer::BeginPlay()
{
	Super::BeginPlay();

	TArray<UPhysicsConstraintComponent*> Constraints;
	GetComponents<UPhysicsConstraintComponent>(Constraints);

	TArray<UActorComponent*> LegActors = GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("Leg"));
	for (UActorComponent* Comp : LegActors)
	{
		if (UStaticMeshComponent* Leg = Cast<UStaticMeshComponent>(Comp))
		{
			Legs.Add(Leg);
		}
	}

	for (auto* C : Constraints)
	{
		if (C->GetName() == "Joint_FL")
		{
			JointFL = C;
		}
		else if (C->GetName() == "Joint_FR")
		{
			JointFR = C;
		}
		else if (C->GetName() == "Joint_BL")
		{
			JointBL = C;
		}
		else if (C->GetName() == "Joint_BR")
		{
			JointBR = C;
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

	JointFL->SetAngularVelocityTarget(FVector(0.0f, VelocityTargetFL, 0.0f));
	JointFR->SetAngularVelocityTarget(FVector(0.0f, VelocityTargetFR, 0.0f));
	JointBL->SetAngularVelocityTarget(FVector(0.0f, VelocityTargetBL, 0.0f));
	JointBR->SetAngularVelocityTarget(FVector(0.0f, VelocityTargetBR, 0.0f));

	for (UStaticMeshComponent* Leg : Legs)
	{
		Leg->WakeRigidBody();
	}
}

void ARunnersPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LeftForward", IE_Pressed, this, &ARunnersPlayer::LeftForwardOn);
	PlayerInputComponent->BindAction("LeftBack", IE_Pressed, this, &ARunnersPlayer::LeftBackOn);

	PlayerInputComponent->BindAction("RightForward", IE_Pressed, this, &ARunnersPlayer::RightForwardOn);
	PlayerInputComponent->BindAction("RightBack", IE_Pressed, this, &ARunnersPlayer::RightBackOn);
}

void ARunnersPlayer::SetVelocityTargetFL(float target)
{
	VelocityTargetFL = FMath::Clamp(target, -1.0f, 1.0f);
}

void ARunnersPlayer::SetVelocityTargetFR(float target)
{
	VelocityTargetFR = FMath::Clamp(target, -1.0f, 1.0f);
}

void ARunnersPlayer::SetVelocityTargetBL(float target)
{
	VelocityTargetBL = FMath::Clamp(target, -1.0f, 1.0f);
}

void ARunnersPlayer::SetVelocityTargetBR(float target)
{
	VelocityTargetBR = FMath::Clamp(target, -1.0f, 1.0f);
}

void ARunnersPlayer::LeftForwardOn()
{
	VelocityTargetFL += 0.5f;
	UE_LOG(LogTemp, Display, TEXT("left forward %f"), VelocityTargetFL);
}

void ARunnersPlayer::LeftBackOn()
{
	VelocityTargetFL -= 0.5f;
	UE_LOG(LogTemp, Display, TEXT("left backward %f"), VelocityTargetFL);
}

void ARunnersPlayer::RightForwardOn()
{
	VelocityTargetFR += 0.5f;
	UE_LOG(LogTemp, Display, TEXT("left forward %f"), VelocityTargetFR);
}

void ARunnersPlayer::RightBackOn()
{
	VelocityTargetFR -= 0.5f;
	UE_LOG(LogTemp, Display, TEXT("left forward %f"), VelocityTargetFR);
}
