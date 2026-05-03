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

private:
	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* Joint_1;

	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* Joint_2;

	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* Joint_3;

	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* Joint_4;

	UPROPERTY(VisibleAnywhere)
	TArray<UStaticMeshComponent*> Arms;

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
