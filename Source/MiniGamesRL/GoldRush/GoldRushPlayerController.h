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
	void ToggleZoom();

	TWeakObjectPtr<ACameraActor> LevelCamera;

	bool bIsZooming = false;
	bool bIsZoomingIn = false;
	float ZoomElapsed = 0.0f;
	float ZoomStartWidth = 0.0f;
	float ZoomTargetWidth = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomFar = 13000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomNear = 3200.0f;
};
