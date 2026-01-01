#include "BushDatabase.h"

DEFINE_LOG_CATEGORY_STATIC(LogBushDatabase, Log, All);

void UBushDatabase::AddBush(const FBushMetadata& Bush)
{
    const bool bExists = Bushes.ContainsByPredicate([&Bush](const FBushMetadata& Existing)
    {
        return Existing.BushName == Bush.BushName;
    });
    if (!bExists)
    {
        Bushes.Add(Bush);
        UE_LOG(LogBushDatabase, Display, TEXT("Added bush: %s (Type=%s)"), *Bush.BushName.ToString(), *Bush.BushType.ToString());
    }
}

void UBushDatabase::RemoveBush(const FBushMetadata& Bush)
{
    const int32 Index = Bushes.IndexOfByPredicate([&Bush](const FBushMetadata& Existing)
    {
        return Existing.BushName == Bush.BushName;
    });
    if (Index != INDEX_NONE)
    {
        Bushes.RemoveAt(Index);
        UE_LOG(LogBushDatabase, Display, TEXT("Removed bush: %s"), *Bush.BushName.ToString());
    }
}

bool UBushDatabase::FindBushByName(FName Name, FBushMetadata& OutBush) const
{
    const FBushMetadata* Found = Bushes.FindByPredicate([Name](const FBushMetadata& B)
    {
        return B.BushName == Name;
    });
    if (Found)
    {
        OutBush = *Found;
        return true;
    }
    return false;
}

TArray<FBushMetadata> UBushDatabase::FindBushesByTag(FName Tag) const
{
    TArray<FBushMetadata> Results;
    for (const FBushMetadata& B : Bushes)
    {
        if (B.Tags.Contains(Tag))
        {
            Results.Add(B);
        }
    }
    return Results;
}

TArray<FBushMetadata> UBushDatabase::FindBushesByType(FName Type) const
{
    TArray<FBushMetadata> Results;
    for (const FBushMetadata& B : Bushes)
    {
        if (B.BushType == Type)
        {
            Results.Add(B);
        }
    }
    return Results;
}
