// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "TanksShell.generated.h"

UCLASS()
class MINIGAMESRL_API ATanksShell : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATanksShell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UFUNCTION()
	void Launch(const FVector& Direction, float Speed, AActor* Tank);

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	           const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "TanksConfig")
	UNiagaraSystem* ExplosionEffect;
};
