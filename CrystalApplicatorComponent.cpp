#include "CrystalApplicatorComponent.h"

#include "Components/Image.h"
#include "Components/MeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Texture2D.h"

UCrystalApplicatorComponent::UCrystalApplicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCrystalApplicatorComponent::ApplyCrystal(const FString& CrystalName)
{
	if (!CrystalDatabase)
	{
		UE_LOG(LogTemp, Warning, TEXT("CrystalApplicator: No CrystalDatabase assigned."));
		return;
	}

	FCrystalData Data;
	if (CrystalDatabase->FindCrystalByName(CrystalName, Data) && Data.CrystalFile)
	{
		ApplyToUI(Data.CrystalFile);
		ApplyToMesh(Data.CrystalFile);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Crystal '%s' not found in the database."), *CrystalName);
	}
}

void UCrystalApplicatorComponent::ApplyRandomCrystalByTag(const FString& Tag)
{
	if (!CrystalDatabase)
	{
		UE_LOG(LogTemp, Warning, TEXT("CrystalApplicator: No CrystalDatabase assigned."));
		return;
	}

	TArray<FCrystalData> Matches = CrystalDatabase->FindCrystalsByTag(Tag);
	if (Matches.Num() > 0)
	{
		const int32 Index = FMath::RandRange(0, Matches.Num() - 1);
		ApplyCrystal(Matches[Index].CrystalName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No crystals with tag '%s' found."), *Tag);
	}
}

void UCrystalApplicatorComponent::ApplyToUI(UTexture2D* Texture)
{
	if (TargetWidgetImage && Texture)
	{
		TargetWidgetImage->SetBrushFromTexture(Texture, true);
	}
}

void UCrystalApplicatorComponent::ApplyToMesh(UTexture2D* Texture)
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
