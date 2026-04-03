// Guchan Alkan - Licensed under GPLv3

#include "GoldRush/GoldRushPlayerController.h"
#include "EngineUtils.h"
#include "Camera/CameraComponent.h"

AGoldRushPlayerController::AGoldRushPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGoldRushPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Without this input does not reach the game, not sure why
	SetInputMode(FInputModeGameOnly());

	for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
	{
		LevelCamera = *It;
		break;
	}
}

void AGoldRushPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	FInputKeyBinding& Binding = InputComponent->BindKey(
		EKeys::SpaceBar, IE_Pressed, this, &AGoldRushPlayerController::ToggleZoom
	);
}

void AGoldRushPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsZooming || !LevelCamera.IsValid())
	{
		return;
	}

	ZoomElapsed += DeltaTime;
	const float Alpha = FMath::Clamp(ZoomElapsed / ZoomDuration, 0.0f, 1.0f);
	LevelCamera->GetCameraComponent()->SetOrthoWidth(FMath::Lerp(ZoomStartWidth, ZoomTargetWidth, Alpha));

	if (Alpha >= 1.0f)
	{
		bIsZooming = false;
	}
}

void AGoldRushPlayerController::ToggleZoom()
{
	if (!LevelCamera.IsValid())
	{
		return;
	}

	ZoomElapsed = 0.0f;
	ZoomStartWidth = bIsZoomingIn ? ZoomFar : ZoomNear;
	ZoomTargetWidth = bIsZoomingIn ? ZoomNear : ZoomFar;
	bIsZoomingIn = !bIsZoomingIn;
	bIsZooming = true;
}
