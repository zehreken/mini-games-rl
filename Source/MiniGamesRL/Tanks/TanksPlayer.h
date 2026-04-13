// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "TanksPlayer.generated.h"

UCLASS()
class MINIGAMESRL_API ATanksPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATanksPlayer();

	void ResetAgent();

	FVector GetActorPreviousLocation() const;
	
	UPROPERTY(VisibleInstanceOnly)
	FVector TargetLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetThrottle(float LeftThrottle, float RightThrottle);

private:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Body;

	UPROPERTY(VisibleInstanceOnly)
	FVector PreviousLocation;

	UPROPERTY(EditAnywhere, Category = "Tank")
	float LinearDamping = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Tank")
	float AngularDamping = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Tank")
	float SpringStiffness = 3000.f;

	UPROPERTY(EditAnywhere, Category = "Tank")
	float SpringDamping = 500.f;

	UPROPERTY(EditAnywhere, Category = "Tank")
	float RestLength = 100.f;

	UPROPERTY()
	TArray<UStaticMeshComponent*> Wheels;

	UPROPERTY()
	TArray<FVector> WheelRestPositions;

	TArray<FVector> CornerOffsets = {
		FVector(100.f, 50.f, 0.f),
		FVector(100.f, -50.f, 0.f),
		FVector(-100.f, 50.f, 0.f),
		FVector(-100.f, -50.f, 0.f),
	};

	float LeftInput;
	float RightInput;

	void LeftForwardOn();
	void LeftForwardOff();
	void LeftBackOn();
	void LeftBackOff();

	void RightForwardOn();
	void RightForwardOff();
	void RightBackOn();
	void RightBackOff();

	void ApplySuspension();
	void MoveWheels();
};
