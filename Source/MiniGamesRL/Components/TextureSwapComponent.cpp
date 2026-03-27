// Guchan Alkan - Licensed under GPLv3

#include "Components/TextureSwapComponent.h"
#include "Components/StaticMeshComponent.h"

UTextureSwapComponent::UTextureSwapComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTextureSwapComponent::BeginPlay()
{
	Super::BeginPlay();
	InitialiseMaterial();
}

void UTextureSwapComponent::SwapTexture(int32 Index)
{
	if (Textures[Index].Priority < Textures[PreviousIndex].Priority)
		return;
	
	GetWorld()->GetTimerManager().ClearTimer(RevertTimerHandle);

	// Index 0 is the default — just apply it with no timer.
	if (Index <= 0)
	{
		ApplyTexture(0);
		return;
	}

	ApplyTexture(Index);

	if (!Textures.IsValidIndex(Index) || Textures[Index].Duration <= 0.f)
	{
		return;
	}

	FTimerDelegate RevertDelegate;
	RevertDelegate.BindUObject(this, &UTextureSwapComponent::RevertToDefault);
	GetWorld()->GetTimerManager().SetTimer(RevertTimerHandle, RevertDelegate,
	                                       Textures[Index].Duration, false);
}

void UTextureSwapComponent::InitialiseMaterial()
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;

	UStaticMeshComponent* Mesh = Owner->FindComponentByClass<UStaticMeshComponent>();
	if (!Mesh)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("UTextureSwapComponent on '%s': no UStaticMeshComponent found."),
		       *Owner->GetName());
		return;
	}

	DynMat = Mesh->CreateAndSetMaterialInstanceDynamic(MaterialSlotIndex);
	if (!DynMat)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("UTextureSwapComponent on '%s': failed to create dynamic material "
		            "at slot %d. Check that a material is assigned to the mesh."),
		       *Owner->GetName(), MaterialSlotIndex);
		return;
	}

	ApplyTexture(0);
}

void UTextureSwapComponent::ApplyTexture(int32 Index)
{
	if (!IsValid(DynMat)) return;
	if (!DynMat) return;
	if (!Textures.IsValidIndex(Index) || !Textures[Index].Texture) return;

	DynMat->SetTextureParameterValue(TextureParameterName, Textures[Index].Texture);
	PreviousIndex = Index;
}

void UTextureSwapComponent::RevertToDefault()
{
	ApplyTexture(0);
}
