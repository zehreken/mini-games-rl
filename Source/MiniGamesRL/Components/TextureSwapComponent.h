// Guchan Alkan - Licensed under GPLv3

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TextureSwapComponent.generated.h"

USTRUCT(BlueprintType)
struct FTextureSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextureSlot")
	UTexture2D* Texture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextureSlot",
	          meta = (ClampMin = "0.0"))
	float Duration = 1.0f;

	// Higher is high priority
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextureSlot")
	int32 Priority = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINIGAMESRL_API UTextureSwapComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTextureSwapComponent();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextureSwap")
	TArray<FTextureSlot> Textures;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextureSwap")
	FName TextureParameterName = FName("EmojiTexture");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextureSwap")
	int32 MaterialSlotIndex = 0;

	UFUNCTION(BlueprintCallable, Category = "TextureSwap")
	void SwapTexture(int32 Index);

private:
	UPROPERTY()
	UMaterialInstanceDynamic* DynMat = nullptr;

	FTimerHandle RevertTimerHandle;

	int32 PreviousIndex = 0;

	void InitialiseMaterial();
	void ApplyTexture(int32 Index);
	void RevertToDefault();
};
