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

	// TotalTime += DeltaTime;
	JointFL->SetAngularVelocityTarget(FVector(0.0f, TotalTime, 0.0f));
	JointFR->SetAngularVelocityTarget(FVector(0.0f, TotalTime, 0.0f));
	JointBL->SetAngularVelocityTarget(FVector(0.0f, -TotalTime * 100.0f, 0.0f));
	JointBR->SetAngularVelocityTarget(FVector(0.0f, -TotalTime * 100.0f, 0.0f));

	for (UStaticMeshComponent* Leg : Legs)
	{
		Leg->WakeRigidBody();
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
