#include "EventMemoryUpdater.h"

#include "Misc/Char.h"

void FEventMemoryUpdater::RecordEvent(const FString& EventDescription, const FVector& Location, const TArray<float>& Embedding)
{
    const FString LocationStr = FString::Printf(TEXT("(%g, %g, %g)"), Location.X, Location.Y, Location.Z);
    const FString MemoryKey = EventDescription + TEXT("@") + LocationStr;
    MemoryCache.AddPrompt(MemoryKey, Embedding, /*InitialPriority*/1.0f, /*TTL*/600.0f);
    UE_LOG(LogTemp, Log, TEXT("Event Recorded: %s"), *MemoryKey);
}

FString FEventMemoryUpdater::RecallRelevantMemory(const FVector& QueryLocation, float ProximityThreshold)
{
    const FString Highest = MemoryCache.GetHighestPriorityPrompt();
    if (Highest.IsEmpty())
    {
        return TEXT("No relevant memories found.");
    }

    FString Left, Right;
    if (Highest.Split(TEXT("@"), &Left, &Right))
    {
        const FVector Loc = ParseLocation(Right);
        const float Dist = FVector::Dist(QueryLocation, Loc);
        if (Dist < ProximityThreshold)
        {
            return Highest;
        }
    }

    return TEXT("No relevant memories found.");
}

FVector FEventMemoryUpdater::ParseLocation(const FString& LocationStr)
{
    FString Clean = LocationStr;
    Clean.ReplaceInline(TEXT("("), TEXT(""));
    Clean.ReplaceInline(TEXT(")"), TEXT(""));

    TArray<FString> Parts;
    Clean.ParseIntoArray(Parts, TEXT(","), /*CullEmpty*/true);
    if (Parts.Num() != 3)
    {
        return FVector::ZeroVector;
    }

    float X = 0.f, Y = 0.f, Z = 0.f;
    Parts[0] = Parts[0].TrimStartAndEnd();
    Parts[1] = Parts[1].TrimStartAndEnd();
    Parts[2] = Parts[2].TrimStartAndEnd();

    if (!LexTryParseString(X, *Parts[0]) || !LexTryParseString(Y, *Parts[1]) || !LexTryParseString(Z, *Parts[2]))
    {
        return FVector::ZeroVector;
    }

    return FVector(X, Y, Z);
}

