// Guchan Alkan - Licensed under GPLv3


#include "Tanks/TanksPlayer.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "Tanks/TanksShell.h"
#include "Tanks/TanksGameMode.h"


// Sets default values
ATanksPlayer::ATanksPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Body = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	RootComponent = Body;
	Body->SetBoxExtent(FVector(100.0f, 50.0f, 25.0f)); // half-extents in cm
	Body->SetCollisionProfileName(TEXT("BlockAll"));
	Body->SetSimulatePhysics(true);

	LeftInput = 0.0f;
	RightInput = 0.0f;

	ResetAgent();
}

void ATanksPlayer::ResetAgent()
{
	FVector RandomLocation = FMath::VRand();
	SetActorLocation(FVector(RandomLocation.X * 500.0f, RandomLocation.Y * 500.0f, 200.0f));
	FRotator RandomRotation = FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f);
	SetActorRotation(RandomRotation);
	bHasArrived = false;
	bShellHit = false;
	bCanShoot = true;
	bHasShot = false;
}

FVector ATanksPlayer::GetActorPreviousLocation() const
{
	return PreviousLocation;
}

// Called when the game starts or when spawned
void ATanksPlayer::BeginPlay()
{
	Super::BeginPlay();

	Target = GetWorld()->SpawnActor<ATanksTarget>(TargetClass);

	ShellTarget = GetWorld()->SpawnActor<ATanksShellTarget>(ShellTargetClass);

	Body->SetLinearDamping(LinearDamping);
	Body->SetAngularDamping(AngularDamping);

	TArray<UActorComponent*> WheelActors = GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("Wheel"));
	for (UActorComponent* Comp : WheelActors)
	{
		if (UStaticMeshComponent* Wheel = Cast<UStaticMeshComponent>(Comp))
		{
			Wheels.Add(Wheel);
		}
	}
	
	TArray<UActorComponent*> GunComponents = GetComponentsByTag(USceneComponent::StaticClass(), FName("Gun"));
	if (GunComponents.Num() > 0)
	{
		GunComponent = Cast<USceneComponent>(GunComponents[0]);
	}

	WheelRestPositions.Empty();
	for (UStaticMeshComponent* Wheel : Wheels)
	{
		// RelativeLocation is already in local space relative to the parent
		WheelRestPositions.Add(Wheel->GetRelativeLocation());
	}

	GetWorldTimerManager().SetTimer(
		ShootTimerHandle,
		[this]() { bCanShoot = true; },
		ShootPeriod,
		true);
	bCanShoot = true;
}

// Called every frame
void ATanksPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	constexpr float TrackForce = 200'000.0f;
	constexpr float HalfWidth = 50.0f;
	FVector LeftForce = GetActorForwardVector() * LeftInput * TrackForce;
	FVector RightForce = GetActorForwardVector() * RightInput * TrackForce;

	FVector LeftPos = GetActorLocation() + GetActorRightVector() * -HalfWidth;
	FVector RightPos = GetActorLocation() + GetActorRightVector() * HalfWidth;

	if (!Body) return;

	PreviousLocation = GetActorLocation();

	ApplySuspension();
	MoveWheels();

	Body->AddForceAtLocation(LeftForce, LeftPos);
	Body->AddForceAtLocation(RightForce, RightPos);

	constexpr float ForceDrawScale = 1.0f / 1000.0f;
	DrawDebugDirectionalArrow(GetWorld(), LeftPos, LeftPos + LeftForce * ForceDrawScale, 20.0f, FColor::Blue, false,
	                          -1.0f, 0, 2.0f);
	DrawDebugDirectionalArrow(GetWorld(), RightPos, RightPos + RightForce * ForceDrawScale, 20.0f, FColor::Red, false,
	                          -1.0f, 0, 2.0f);
}

// Called to bind functionality to input
void ATanksPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LeftForward", IE_Pressed, this, &ATanksPlayer::LeftForwardOn);
	PlayerInputComponent->BindAction("LeftForward", IE_Released, this, &ATanksPlayer::LeftForwardOff);
	PlayerInputComponent->BindAction("LeftBack", IE_Pressed, this, &ATanksPlayer::LeftBackOn);
	PlayerInputComponent->BindAction("LeftBack", IE_Released, this, &ATanksPlayer::LeftBackOff);

	PlayerInputComponent->BindAction("RightForward", IE_Pressed, this, &ATanksPlayer::RightForwardOn);
	PlayerInputComponent->BindAction("RightForward", IE_Released, this, &ATanksPlayer::RightForwardOff);
	PlayerInputComponent->BindAction("RightBack", IE_Pressed, this, &ATanksPlayer::RightBackOn);
	PlayerInputComponent->BindAction("RightBack", IE_Released, this, &ATanksPlayer::RightBackOff);

	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &ATanksPlayer::Shoot);
}

void ATanksPlayer::SetThrottle(float LeftThrottle, float RightThrottle)
{
	LeftInput = LeftThrottle;
	RightInput = RightThrottle;
}

void ATanksPlayer::LeftForwardOn()
{
	UE_LOG(LogTemp, Display, TEXT("left forward on"));
	LeftInput = 1.0f;
}

void ATanksPlayer::LeftForwardOff()
{
	LeftInput = 0.0f;
}

void ATanksPlayer::LeftBackOn()
{
	LeftInput = -1.0f;
}

void ATanksPlayer::LeftBackOff()
{
	LeftInput = 0.0f;
}

void ATanksPlayer::RightForwardOn()
{
	RightInput = 1.0f;
}

void ATanksPlayer::RightForwardOff()
{
	RightInput = 0.0f;
}

void ATanksPlayer::RightBackOn()
{
	RightInput = -1.0f;
}

void ATanksPlayer::RightBackOff()
{
	RightInput = 0.0f;
}

void ATanksPlayer::ApplySuspension()
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	for (const FVector& CornerOffset : CornerOffsets)
	{
		FVector CornerWorld = GetActorTransform().TransformPosition(CornerOffset);
		FVector TraceEnd = CornerWorld + FVector::DownVector * RestLength;

		FHitResult Hit;
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, CornerWorld, TraceEnd, ECC_WorldStatic, Params);
		if (bHit)
		{
			float Compression = RestLength - Hit.Distance;

			float SpringForce = Compression * SpringStiffness;

			FVector PointVelocity = Body->GetPhysicsLinearVelocityAtPoint(CornerWorld);
			float DampingForce = PointVelocity.Z * SpringDamping;

			float TotalForce = SpringForce - DampingForce;

			if (TotalForce > 0.0f)
			{
				Body->AddForceAtLocation(FVector::UpVector * TotalForce, CornerWorld);
				// constexpr float SuspensionDrawScale = 1.0f / 100.0f;
				// DrawDebugDirectionalArrow(GetWorld(), CornerWorld,
				//                           CornerWorld + FVector::UpVector * TotalForce * SuspensionDrawScale, 10.0f,
				//                           FColor::Green, false, -1.0f, 0, 1.5f);
			}
		}
	}
}

void ATanksPlayer::MoveWheels()
{
	ATanksGameMode* GameMode = Cast<ATanksGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode) || !GameMode->GetLearningManager()->RunInference) return;
	for (int32 i = 0; i < Wheels.Num(); i++)
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		FVector WorldRestPos = GetActorTransform().TransformPosition(WheelRestPositions[i]);

		FVector TraceStart = WorldRestPos + FVector::UpVector * 50.0f;
		FVector TraceEnd = WorldRestPos + FVector::DownVector * 1000.0f;

		FHitResult Hit;
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldStatic, Params);
		if (bHit)
		{
			// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, -1.0f, 0, 2.0f);
			// DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 10.0f, 8, FColor::Yellow, false, -1.0f);

			Wheels[i]->SetWorldLocation(FVector(Hit.ImpactPoint + FVector::UpVector * 25.0f));
		}
	}
}

void ATanksPlayer::Shoot()
{
	if (!bCanShoot)
		return;

	bCanShoot = false;

	UE_LOG(LogTemp, Display, TEXT("Bam!"));
	FTransform SpawnTransform = FTransform(FRotator::ZeroRotator,
	                                       GetActorLocation() - GetActorForwardVector() * 50.0f + FVector(
		                                       0.0f, 0.0f, 50.0f));
	ATanksShell* Shell = GetWorld()->SpawnActor<ATanksShell>(ShellClass, SpawnTransform);
	MoveIgnoreActorAdd(Shell);
	if (!IsValid(Shell)) return;
	FVector RandomDirection = FVector::ForwardVector * 300.0f; //FMath::VRand();
	FVector ClampedDirection = FVector(RandomDirection.X, RandomDirection.Y, FMath::Abs(RandomDirection.Z));
	Shell->Launch(ClampedDirection, 1500.0f, this);

	// Temp ========
	float Reward = 0.0f;
	float Gravity = FMath::Abs(GetWorld()->GetGravityZ());
	FVector ToTarget = ShellTarget->GetActorLocation() - GunComponent->GetComponentLocation();
	float HorizDist = FVector2D(ToTarget.X, ToTarget.Y).Size();
	float HeightDiff = ToTarget.Z;
	float Speed = 1500.0f;
	float SpeedSq = Speed * Speed;

	float Discriminant = SpeedSq * SpeedSq - Gravity * (Gravity * HorizDist * HorizDist + 2.f * HeightDiff *
		SpeedSq);
	if (Discriminant >= 0.f)
	{
		float Angle = FMath::Atan2(SpeedSq - FMath::Sqrt(Discriminant), Gravity * HorizDist);
		FVector2D Horizontal = FVector2D(ToTarget.X, ToTarget.Y).GetSafeNormal();
		FVector IdealDir = FVector(Horizontal.X * FMath::Cos(Angle),
								   Horizontal.Y * FMath::Cos(Angle),
								   FMath::Sin(Angle));
		float Alignment = FVector::DotProduct(ClampedDirection.GetSafeNormal(), IdealDir);
		Reward += FMath::Pow(Alignment, 20.0f);
	}
	else
	{
		Reward -= 1.0f;
	}
		
	UE_LOG(LogTemp, Display, TEXT("shell reward: %f"), Reward);
}


void ATanksPlayer::ShootAt(const FVector& Direction)
{
	if (!bCanShoot)
		return;

	bCanShoot = false;
	bHasShot = true;

	// UE_LOG(LogTemp, Display, TEXT("Bam!"));
	FTransform SpawnTransform = FTransform(FRotator::ZeroRotator,
	                                       GetActorLocation() - GetActorForwardVector() * 50.0f + FVector(
		                                       0.0f, 0.0f, 50.0f));
	ATanksShell* Shell = GetWorld()->SpawnActor<ATanksShell>(ShellClass, SpawnTransform);
	MoveIgnoreActorAdd(Shell);
	if (!IsValid(Shell)) return;
	FVector ClampedDirection = FVector(Direction.X, Direction.Y, FMath::Abs(Direction.Z));
	LastFiredDirection = ClampedDirection;
	Shell->Launch(ClampedDirection, 1500.0f, this);
}

void ATanksPlayer::SetShellHit(const FVector& Location)
{
	if (!bShellHit) // Added this guard because there is timing issue, first caller wins
	{
		bShellHit = true;
		ShellHitDelta = ShellTarget->GetActorLocation() - Location;
		UE_LOG(LogTemp, Display, TEXT("Target: %s  Impact: %s  Delta: %s  Len: %.1f"),
			*ShellTarget->GetActorLocation().ToString(), *Location.ToString(),
			*ShellHitDelta.ToString(), ShellHitDelta.Length());
	}
}

float ATanksPlayer::GetNormalizedShootTime() const
{
	return GetWorld()->GetTimerManager().GetTimerElapsed(ShootTimerHandle) / ShootPeriod;
}
