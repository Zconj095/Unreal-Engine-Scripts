#include "ParticleTextureApplicatorComponent.h"

#include "Components/PrimitiveComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

UParticleTextureApplicatorComponent::UParticleTextureApplicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UParticleTextureApplicatorComponent::ApplyTexture(const FString& TextureName)
{
	if (!TextureDatabase || !TargetParticleSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("ParticleTextureApplicator: Missing TextureDatabase or TargetParticleSystem."));
		return;
	}

	FParticleTextureData Data;
	if (TextureDatabase->FindTextureByName(TextureName, Data) && Data.TextureFile)
	{
		ApplyToParticles(Data.TextureFile);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Texture '%s' not found in the database."), *TextureName);
	}
}

void UParticleTextureApplicatorComponent::ApplyRandomTextureByTag(const FString& Tag)
{
	if (!TextureDatabase || !TargetParticleSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("ParticleTextureApplicator: Missing TextureDatabase or TargetParticleSystem."));
		return;
	}

	TArray<FParticleTextureData> Matches = TextureDatabase->FindTexturesByTag(Tag);
	if (Matches.Num() > 0)
	{
		const int32 Index = FMath::RandRange(0, Matches.Num() - 1);
		ApplyTexture(Matches[Index].TextureName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No textures with tag '%s' found."), *Tag);
	}
}

void UParticleTextureApplicatorComponent::ApplyToParticles(UTexture2D* Texture)
{
	if (!TargetParticleSystem || !Texture)
	{
		return;
	}

	const int32 NumMaterials = TargetParticleSystem->GetNumMaterials();
	for (int32 Index = 0; Index < NumMaterials; ++Index)
	{
		UMaterialInstanceDynamic* MID = TargetParticleSystem->CreateAndSetMaterialInstanceDynamic(Index);
		if (MID)
		{
			MID->SetTextureParameterValue(TextureParameterName, Texture);
		}
	}
}
