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

	TArray<UActorComponent*> BodyActors = GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("MainBody"));
	if (BodyActors.Num() > 0)
	{
		MainBody = Cast<UStaticMeshComponent>(BodyActors[0]);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("no mainbody found"));
	}

	SpawnFurShells();

	
	TArray<UActorComponent*> LegActors = GetComponentsByTag(UCapsuleComponent::StaticClass(), FName("Leg"));
	for (UActorComponent* Comp : LegActors)
	{
		if (UCapsuleComponent* Leg = Cast<UCapsuleComponent>(Comp))
		{
			Legs.Add(Leg);
			LegLocations.Add(Leg->GetComponentLocation());
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

	float Angle = FMath::RandRange(0.0f, 2.0f * PI);
	LookAtDirection = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f);
}

void ARunnersPlayer::SpawnFurShells()
{
	if (!MainBody || !FurMaterial) return;

	for (int32 i = 0; i < ShellCount; i++)
	{
		UStaticMeshComponent* Shell = NewObject<UStaticMeshComponent>(this);
		Shell->SetStaticMesh(MainBody->GetStaticMesh());
		Shell->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Shell->SetSimulatePhysics(false);
		Shell->AttachToComponent(MainBody, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Shell->RegisterComponent();
		bool bShouldCastShadow = (i >= ShellCount - 7) || i < 1; // Keep the body shadow and add some of the top layers
		Shell->SetCastShadow(bShouldCastShadow);
		Shell->bCastDynamicShadow = bShouldCastShadow;
		Shell->bCastStaticShadow = bShouldCastShadow;
		Shell->bCastHiddenShadow = bShouldCastShadow;
		Shell->bCastShadowAsTwoSided = bShouldCastShadow;

		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(FurMaterial, this);
		float ShellValue = (float)i / (float)ShellCount;
		MID->SetScalarParameterValue(FName("ShellIndex"), ShellValue);
		UE_LOG(LogTemp, Display, TEXT("Shell %d ShellIndex: %f"), i, ShellValue);
		Shell->SetMaterial(0, MID);

		FurShells.Add(Shell);
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
	
	FVector LookAtStart = GetActorLocation() + FVector::UpVector * 100.0f;
	FVector LookAtEnd = LookAtStart + LookAtDirection * 100.0f;
	DrawDebugDirectionalArrow(GetWorld(), LookAtStart, LookAtEnd, 10.0f, FColor::Green, false,
							  -1.0f, 0.0f, 1.0f);

	JointFL->SetAngularVelocityTarget(FVector(0.0f, VelocityTargetFL, 0.0f));
	JointFR->SetAngularVelocityTarget(FVector(0.0f, VelocityTargetFR, 0.0f));
	JointBL->SetAngularVelocityTarget(FVector(0.0f, VelocityTargetBL, 0.0f));
	JointBR->SetAngularVelocityTarget(FVector(0.0f, VelocityTargetBR, 0.0f));

	for (UCapsuleComponent* Leg : Legs)
	{
		Leg->WakeRigidBody();
	}

	// 1.0 = perfectly aligned, 0.0 = perpendicular, -1.0 = upside down
	float Alignment = FVector::DotProduct(GetActorUpVector(), FVector::UpVector);
	// UE_LOG(LogTemp, Display, TEXT("Alignment: %f"), Alignment);
	if (Alignment < 0.95f)
	{
		bHasFlipped = true;
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

void ARunnersPlayer::ResetAgent(int32 AgentId)
{
	// UE_LOG(LogTemp, Display, TEXT("Agent id: %d"), AgentId);
	MainBody->SetSimulatePhysics(false);
	for (auto Leg : Legs)
	{
		Leg->SetSimulatePhysics(false);
	}
	// Reset bodies
	MainBody->SetPhysicsLinearVelocity(FVector::Zero());
	MainBody->SetPhysicsAngularVelocityInRadians(FVector::Zero());
	for (auto Leg : Legs)
	{
		Leg->SetPhysicsLinearVelocity(FVector::Zero());
		Leg->SetPhysicsAngularVelocityInRadians(FVector::Zero());
	}
	// Reset joints
	JointFL->SetAngularVelocityTarget(FVector::Zero());
	JointFR->SetAngularVelocityTarget(FVector::Zero());
	JointBL->SetAngularVelocityTarget(FVector::Zero());
	JointBR->SetAngularVelocityTarget(FVector::Zero());

	bHasFlipped = false;
	VelocityTargetFL = 0.0f;
	VelocityTargetFR = 0.0f;
	VelocityTargetBL = 0.0f;
	VelocityTargetBR = 0.0f;

	// Break joints
	JointFL->TermComponentConstraint();
	JointFR->TermComponentConstraint();
	JointBL->TermComponentConstraint();
	JointBR->TermComponentConstraint();

	FRotator RandomRotation = FRotator(0.f, 0.0f, 0.f);
	FVector Location = FVector(AgentId / 8 * 1200.0f, AgentId % 8 * 1200.0f, 150.0f);
	SetActorLocationAndRotation(Location, FRotator::ZeroRotator);
	// MainBody->SetWorldLocationAndRotation(FVector(0.0f, 0.0f, 400.0f), FRotator::ZeroRotator);
	for (int32 i = 0; i < Legs.Num(); i++)
	{
		Legs[i]->SetWorldLocationAndRotation(LegLocations[i], FRotator::ZeroRotator);
	}

	// Reconnect joints
	JointFL->InitComponentConstraint();
	JointFR->InitComponentConstraint();
	JointBL->InitComponentConstraint();
	JointBR->InitComponentConstraint();

	MainBody->SetSimulatePhysics(true);
	for (auto Leg : Legs)
	{
		Leg->SetSimulatePhysics(true);
	}

	float Angle = FMath::RandRange(0.0f, 2.0f * PI);
	LookAtDirection = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f);
}

constexpr float VelocityFactor = 10.0f;

void ARunnersPlayer::SetVelocityTargetFL(float target)
{
	VelocityTargetFL = FMath::Clamp(target, -1.0f, 1.0f) * VelocityFactor;
}

void ARunnersPlayer::SetVelocityTargetFR(float target)
{
	VelocityTargetFR = FMath::Clamp(target, -1.0f, 1.0f) * VelocityFactor;
}

void ARunnersPlayer::SetVelocityTargetBL(float target)
{
	// Back joints are rotated 180 degrees
	VelocityTargetBL = FMath::Clamp(target, -1.0f, 1.0f) * VelocityFactor;
}

void ARunnersPlayer::SetVelocityTargetBR(float target)
{
	// Back joints are rotated 180 degrees
	VelocityTargetBR = FMath::Clamp(target, -1.0f, 1.0f) * VelocityFactor;
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
