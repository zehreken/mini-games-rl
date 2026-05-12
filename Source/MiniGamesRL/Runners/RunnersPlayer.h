// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Runners/RunnersTarget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "RunnersPlayer.generated.h"

UCLASS()
class MINIGAMESRL_API ARunnersPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARunnersPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SpawnFurShells();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	bool bHasArrived;

	UPROPERTY(VisibleInstanceOnly)
	FVector LookAtDirection;

	UPROPERTY()
	ARunnersTarget* Target;

	void ResetAgent(int32 AgentId);
	FVector GetActorPreviousLocation() const;

	void SetVelocityTargetFL(float target);
	void SetVelocityTargetFR(float target);
	void SetVelocityTargetBL(float target);
	void SetVelocityTargetBR(float target);

	float GetJointAngleFL() const;
	float GetJointAngleFR() const;
	float GetJointAngleBL() const;
	float GetJointAngleBR() const;
	FVector GetAngularVelocityFL() const;
	FVector GetAngularVelocityFR() const;
	FVector GetAngularVelocityBL() const;
	FVector GetAngularVelocityBR() const;

	bool bHasFlipped; // Termination condition

private:
	UPROPERTY(EditAnywhere, Category = "RunnersConfig")
	TSubclassOf<ARunnersTarget> TargetClass;

	UPROPERTY(VisibleInstanceOnly)
	FVector PreviousLocation;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MainBody;

	UPROPERTY(EditAnywhere, Category="RunnersConfig")
	UStaticMesh* FurMesh;

	UPROPERTY(EditAnywhere, Category="Fur")
	UMaterialInterface* FurMaterial;

	UPROPERTY(EditAnywhere, Category="Fur")
	int32 ShellCount = 16;

	UPROPERTY(EditAnywhere, Category="Fur")
	float ShellSpacing = 0.5f;

	UPROPERTY()
	TArray<UStaticMeshComponent*> FurShells;

	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* JointFL;

	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* JointFR;

	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* JointBL;

	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* JointBR;

	UPROPERTY()
	UCapsuleComponent* LegFL;

	UPROPERTY()
	UCapsuleComponent* LegFR;

	UPROPERTY()
	UCapsuleComponent* LegBL;

	UPROPERTY()
	UCapsuleComponent* LegBR;

	UPROPERTY()
	TArray<UCapsuleComponent*> Legs;

	UPROPERTY()
	TArray<FVector> LegLocations;

	float VelocityTargetFL;
	float VelocityTargetFR;
	float VelocityTargetBL;
	float VelocityTargetBR;

	void LeftForwardOn();
	void LeftBackOn();

	void RightForwardOn();
	void RightBackOn();
};

inline float ARunnersPlayer::GetJointAngleFL() const
{
	return JointFL->GetCurrentSwing2();
}

inline float ARunnersPlayer::GetJointAngleFR() const
{
	return JointFR->GetCurrentSwing2();
}

inline float ARunnersPlayer::GetJointAngleBL() const
{
	return JointBL->GetCurrentSwing2();
}

inline float ARunnersPlayer::GetJointAngleBR() const
{
	return JointBR->GetCurrentSwing2();
}

inline FVector ARunnersPlayer::GetAngularVelocityFL() const
{
	return GetActorTransform().InverseTransformVector(LegFL->GetPhysicsAngularVelocityInRadians());
}

inline FVector ARunnersPlayer::GetAngularVelocityFR() const
{
	return GetActorTransform().InverseTransformVector(LegFR->GetPhysicsAngularVelocityInRadians());
}

inline FVector ARunnersPlayer::GetAngularVelocityBL() const
{
	return GetActorTransform().InverseTransformVector(LegBL->GetPhysicsAngularVelocityInRadians());
}

inline FVector ARunnersPlayer::GetAngularVelocityBR() const
{
	return GetActorTransform().InverseTransformVector(LegBR->GetPhysicsAngularVelocityInRadians());
}
