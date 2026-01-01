#include "CloudApplicatorComponent.h"

#include "CloudDatabase.h"
#include "Components/PrimitiveComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Texture2D.h"

DEFINE_LOG_CATEGORY_STATIC(LogCloudApplicator, Log, All);

bool UCloudApplicatorComponent::ApplyCloudByName(FName CloudName)
{
    if (!CloudDatabase)
    {
        UE_LOG(LogCloudApplicator, Warning, TEXT("ApplyCloudByName: CloudDatabase not set"));
        return false;
    }

    FCloudMetadata Cloud;
    if (!CloudDatabase->FindCloudByName(CloudName, Cloud))
    {
        UE_LOG(LogCloudApplicator, Warning, TEXT("Cloud '%s' not found"), *CloudName.ToString());
        return false;
    }

    UTexture2D* Texture = Cloud.CloudFile.LoadSynchronous();
    if (!Texture)
    {
        UE_LOG(LogCloudApplicator, Warning, TEXT("Cloud '%s' has no texture"), *CloudName.ToString());
        return false;
    }

    return ApplyTextureToTarget(Texture);
}

bool UCloudApplicatorComponent::ApplyRandomCloudByTag(FName Tag)
{
    if (!CloudDatabase)
    {
        UE_LOG(LogCloudApplicator, Warning, TEXT("ApplyRandomCloudByTag: CloudDatabase not set"));
        return false;
    }

    const TArray<FCloudMetadata> Clouds = CloudDatabase->FindCloudsByTag(Tag);
    if (Clouds.Num() <= 0)
    {
        UE_LOG(LogCloudApplicator, Warning, TEXT("No clouds with tag '%s' found"), *Tag.ToString());
        return false;
    }

    const int32 Index = FMath::RandHelper(Clouds.Num());
    const FCloudMetadata& Chosen = Clouds[Index];
    UTexture2D* Texture = Chosen.CloudFile.LoadSynchronous();
    if (!Texture)
    {
        UE_LOG(LogCloudApplicator, Warning, TEXT("Chosen cloud '%s' has no texture"), *Chosen.CloudName.ToString());
        return false;
    }
    return ApplyTextureToTarget(Texture);
}

bool UCloudApplicatorComponent::ApplyTextureToTarget(UTexture2D* Texture)
{
    if (!TargetComponent || !Texture)
    {
        return false;
    }

    UMaterialInterface* BaseMat = TargetComponent->GetMaterial(0);
    if (!BaseMat)
    {
        UE_LOG(LogCloudApplicator, Warning, TEXT("Target has no material to modify"));
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

