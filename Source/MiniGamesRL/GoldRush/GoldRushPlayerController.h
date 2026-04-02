// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraActor.h"
#include "GoldRushPlayerController.generated.h"

UCLASS()
class MINIGAMESRL_API AGoldRushPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AGoldRushPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

private:
	void StartZoom();

	TWeakObjectPtr<ACameraActor> LevelCamera;

	bool bIsZooming = false;
	float ZoomElapsed = 0.f;
	float ZoomStartWidth = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomTargetWidth = 3200.f;
};
