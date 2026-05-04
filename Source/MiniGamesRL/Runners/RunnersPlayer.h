// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetVelocityTargetFL(float target);
	void SetVelocityTargetFR(float target);
	void SetVelocityTargetBL(float target);
	void SetVelocityTargetBR(float target);

private:
	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* JointFL;

	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* JointFR;

	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* JointBL;

	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* JointBR;

	UPROPERTY(VisibleAnywhere)
	TArray<UStaticMeshComponent*> Legs;

	float VelocityTargetFL;
	float VelocityTargetFR;
	float VelocityTargetBL;
	float VelocityTargetBR;

	float TotalTime;

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
};
