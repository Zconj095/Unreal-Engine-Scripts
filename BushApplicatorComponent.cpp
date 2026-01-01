#include "BushApplicatorComponent.h"

#include "BushDatabase.h"
#include "Components/PrimitiveComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Texture2D.h"

DEFINE_LOG_CATEGORY_STATIC(LogBushApplicator, Log, All);

bool UBushApplicatorComponent::ApplyBushByName(FName BushName)
{
    if (!BushDatabase)
    {
        UE_LOG(LogBushApplicator, Warning, TEXT("ApplyBushByName: BushDatabase not set"));
        return false;
    }

    FBushMetadata Bush;
    if (!BushDatabase->FindBushByName(BushName, Bush))
    {
        UE_LOG(LogBushApplicator, Warning, TEXT("Bush '%s' not found"), *BushName.ToString());
        return false;
    }

    UTexture2D* Texture = Bush.BushFile.LoadSynchronous();
    if (!Texture)
    {
        UE_LOG(LogBushApplicator, Warning, TEXT("Bush '%s' has no texture"), *BushName.ToString());
        return false;
    }

    return ApplyTextureToTarget(Texture);
}

bool UBushApplicatorComponent::ApplyRandomBushByTag(FName Tag)
{
    if (!BushDatabase)
    {
        UE_LOG(LogBushApplicator, Warning, TEXT("ApplyRandomBushByTag: BushDatabase not set"));
        return false;
    }

    const TArray<FBushMetadata> Bushes = BushDatabase->FindBushesByTag(Tag);
    if (Bushes.Num() <= 0)
    {
        UE_LOG(LogBushApplicator, Warning, TEXT("No bushes with tag '%s' found"), *Tag.ToString());
        return false;
    }

    const int32 Index = FMath::RandHelper(Bushes.Num());
    const FBushMetadata& Chosen = Bushes[Index];
    UTexture2D* Texture = Chosen.BushFile.LoadSynchronous();
    if (!Texture)
    {
        UE_LOG(LogBushApplicator, Warning, TEXT("Chosen bush '%s' has no texture"), *Chosen.BushName.ToString());
        return false;
    }
    return ApplyTextureToTarget(Texture);
}

bool UBushApplicatorComponent::ApplyTextureToTarget(UTexture2D* Texture)
{
    if (!TargetComponent || !Texture)
    {
        return false;
    }

    // Create a MID for the first material and set a texture parameter
    UMaterialInterface* BaseMat = TargetComponent->GetMaterial(0);
    if (!BaseMat)
    {
        UE_LOG(LogBushApplicator, Warning, TEXT("Target has no material to modify"));
        return false;
    }

    UMaterialInstanceDynamic* MID = TargetComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BaseMat);
    if (!MID)
    {
        return false;
    }

    MID->SetTextureParameterValue(TextureParameterName, Texture);
    return true;
}

