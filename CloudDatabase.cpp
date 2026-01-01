#include "CloudDatabase.h"

DEFINE_LOG_CATEGORY_STATIC(LogCloudDatabase, Log, All);

void UCloudDatabase::AddCloud(const FCloudMetadata& Cloud)
{
    const bool bExists = Clouds.ContainsByPredicate([&Cloud](const FCloudMetadata& Existing)
    {
        return Existing.CloudName == Cloud.CloudName;
    });
    if (!bExists)
    {
        Clouds.Add(Cloud);
        UE_LOG(LogCloudDatabase, Display, TEXT("Added cloud: %s"), *Cloud.CloudName.ToString());
    }
}

void UCloudDatabase::RemoveCloud(const FCloudMetadata& Cloud)
{
    const int32 Index = Clouds.IndexOfByPredicate([&Cloud](const FCloudMetadata& Existing)
    {
        return Existing.CloudName == Cloud.CloudName;
    });
    if (Index != INDEX_NONE)
    {
        Clouds.RemoveAt(Index);
        UE_LOG(LogCloudDatabase, Display, TEXT("Removed cloud: %s"), *Cloud.CloudName.ToString());
    }
}

bool UCloudDatabase::FindCloudByName(FName Name, FCloudMetadata& OutCloud) const
{
    const FCloudMetadata* Found = Clouds.FindByPredicate([Name](const FCloudMetadata& C)
    {
        return C.CloudName == Name;
    });
    if (Found)
    {
        OutCloud = *Found;
        return true;
    }
    return false;
}

TArray<FCloudMetadata> UCloudDatabase::FindCloudsByTag(FName Tag) const
{
    TArray<FCloudMetadata> Results;
    for (const FCloudMetadata& C : Clouds)
    {
        if (C.Tags.Contains(Tag))
        {
            Results.Add(C);
        }
    }
    return Results;
}

TArray<FCloudMetadata> UCloudDatabase::FindCloudsByType(FName Type) const
{
    TArray<FCloudMetadata> Results;
    for (const FCloudMetadata& C : Clouds)
    {
        if (C.CloudType == Type)
        {
            Results.Add(C);
        }
    }
    return Results;
}
