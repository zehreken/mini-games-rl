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
		EKeys::SpaceBar, IE_Pressed, this, &AGoldRushPlayerController::StartZoom
	);

	UE_LOG(LogTemp, Warning, TEXT("Binding registered: %s"),
	       Binding.KeyDelegate.IsBound() ? TEXT("YES") : TEXT("NO"));
}

void AGoldRushPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsZooming || !LevelCamera.IsValid())
	{
		return;
	}

	ZoomElapsed += DeltaTime;
	const float Alpha = FMath::Clamp(ZoomElapsed / ZoomDuration, 0.f, 1.f);
	LevelCamera->GetCameraComponent()->SetOrthoWidth(FMath::Lerp(ZoomStartWidth, ZoomTargetWidth, Alpha));

	if (Alpha >= 1.f)
	{
		bIsZooming = false;
	}
}

void AGoldRushPlayerController::StartZoom()
{
	if (!LevelCamera.IsValid())
	{
		return;
	}

	ZoomElapsed = 0.f;
	ZoomStartWidth = LevelCamera->GetCameraComponent()->OrthoWidth;
	bIsZooming = true;
}
