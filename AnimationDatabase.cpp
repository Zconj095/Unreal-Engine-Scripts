#include "AnimationDatabase.h"

DEFINE_LOG_CATEGORY_STATIC(LogAnimationDB, Log, All);

void UAnimationDatabase::AddAnimation(const FAnimationMetadata& AnimationData)
{
    const bool bExists = Animations.ContainsByPredicate(
        [&AnimationData](const FAnimationMetadata& Existing)
        {
            return Existing.AnimationName == AnimationData.AnimationName;
        }
    );

    if (!bExists)
    {
        Animations.Add(AnimationData);
        UE_LOG(LogAnimationDB, Display, TEXT("Added animation: Name=%s, CreatureType=%s, Tags=%d, Total=%d"),
            *AnimationData.AnimationName.ToString(), *AnimationData.CreatureType.ToString(), AnimationData.Tags.Num(), Animations.Num());
    }
    else
    {
        UE_LOG(LogAnimationDB, Warning, TEXT("Skipped add: Animation already exists with Name=%s"), *AnimationData.AnimationName.ToString());
    }
}

void UAnimationDatabase::RemoveAnimation(const FAnimationMetadata& AnimationData)
{
    const int32 Index = Animations.IndexOfByPredicate(
        [&AnimationData](const FAnimationMetadata& Existing)
        {
            return Existing.AnimationName == AnimationData.AnimationName;
        }
    );

    if (Index != INDEX_NONE)
    {
        Animations.RemoveAt(Index);
        UE_LOG(LogAnimationDB, Display, TEXT("Removed animation: Name=%s, Remaining=%d"), *AnimationData.AnimationName.ToString(), Animations.Num());
    }
    else
    {
        UE_LOG(LogAnimationDB, Warning, TEXT("Remove failed: No animation found with Name=%s"), *AnimationData.AnimationName.ToString());
    }
}

bool UAnimationDatabase::FindAnimationByName(FName Name, FAnimationMetadata& OutAnimation) const
{
    const FAnimationMetadata* Found = Animations.FindByPredicate(
        [Name](const FAnimationMetadata& A)
        {
            return A.AnimationName == Name;
        }
    );

    if (Found)
    {
        OutAnimation = *Found;
        UE_LOG(LogAnimationDB, Display, TEXT("Find by name succeeded: Name=%s"), *Name.ToString());
        return true;
    }

    UE_LOG(LogAnimationDB, Verbose, TEXT("Find by name failed: Name=%s"), *Name.ToString());
    return false;
}

TArray<FAnimationMetadata> UAnimationDatabase::FindAnimationsByCreatureType(FName CreatureType) const
{
    TArray<FAnimationMetadata> Results;
    for (const FAnimationMetadata& A : Animations)
    {
        if (A.CreatureType == CreatureType)
        {
            Results.Add(A);
        }
    }
    UE_LOG(LogAnimationDB, Display, TEXT("Find by creature type: Type=%s, Count=%d"), *CreatureType.ToString(), Results.Num());
    return Results;
}

TArray<FAnimationMetadata> UAnimationDatabase::FindAnimationsByTag(FName Tag) const
{
    TArray<FAnimationMetadata> Results;
    for (const FAnimationMetadata& A : Animations)
    {
        if (A.Tags.Contains(Tag))
        {
            Results.Add(A);
        }
    }
    UE_LOG(LogAnimationDB, Display, TEXT("Find by tag: Tag=%s, Count=%d"), *Tag.ToString(), Results.Num());
    return Results;
}

TArray<FAnimationMetadata> UAnimationDatabase::FindAnimationsByType(FName AnimationType) const
{
    return FindAnimationsByCreatureType(AnimationType);
}
