#include "FlowerApplicatorComponent.h"

#include "Components/Image.h"
#include "Components/MeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Texture2D.h"

UFlowerApplicatorComponent::UFlowerApplicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFlowerApplicatorComponent::ApplyFlower(const FString& FlowerName)
{
	if (!FlowerDatabase)
	{
		UE_LOG(LogTemp, Warning, TEXT("FlowerApplicator: No FlowerDatabase assigned."));
		return;
	}

	FFlowerData Data;
	if (FlowerDatabase->FindFlowerByName(FlowerName, Data) && Data.FlowerFile)
	{
		ApplyToUI(Data.FlowerFile);
		ApplyToMesh(Data.FlowerFile);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Flower '%s' not found in the database."), *FlowerName);
	}
}

void UFlowerApplicatorComponent::ApplyRandomFlowerByTag(const FString& Tag)
{
	if (!FlowerDatabase)
	{
		UE_LOG(LogTemp, Warning, TEXT("FlowerApplicator: No FlowerDatabase assigned."));
		return;
	}

	TArray<FFlowerData> Matches = FlowerDatabase->FindFlowersByTag(Tag);
	if (Matches.Num() > 0)
	{
		const int32 Index = FMath::RandRange(0, Matches.Num() - 1);
		ApplyFlower(Matches[Index].FlowerName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No flowers with tag '%s' found."), *Tag);
	}
}

void UFlowerApplicatorComponent::ApplyToUI(UTexture2D* Texture)
{
	if (TargetImage && Texture)
	{
		TargetImage->SetBrushFromTexture(Texture, true);
	}
}

void UFlowerApplicatorComponent::ApplyToMesh(UTexture2D* Texture)
{
	if (!TargetMesh || !Texture)
	{
		return;
	}

	const int32 NumMaterials = TargetMesh->GetNumMaterials();
	for (int32 i = 0; i < NumMaterials; ++i)
	{
		UMaterialInstanceDynamic* MID = TargetMesh->CreateAndSetMaterialInstanceDynamic(i);
		if (MID)
		{
			MID->SetTextureParameterValue(TextureParameterName, Texture);
		}
	}
}
