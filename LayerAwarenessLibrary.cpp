#include "LayerAwarenessLibrary.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Engine/CollisionProfile.h"

void ULayerAwarenessLibrary::GetActorsByChannel(UObject* WorldContextObject, TEnumAsByte<ECollisionChannel> Channel, float Radius, FVector Position, TArray<AActor*>& OutActors)
{
    OutActors.Reset();
    if (!WorldContextObject)
    {
        return;
    }

    // Build object type list from channel
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(Channel.GetValue()));

    TArray<AActor*> Ignored;
    UKismetSystemLibrary::SphereOverlapActors(WorldContextObject, Position, Radius, ObjectTypes, AActor::StaticClass(), Ignored, OutActors);
}

FString ULayerAwarenessLibrary::DescribeActorsInChannel(TEnumAsByte<ECollisionChannel> Channel, const TArray<AActor*>& Actors)
{
    const FName ChannelName = UCollisionProfile::Get()->ReturnChannelNameFromContainerIndex(static_cast<int32>(Channel.GetValue()));
    FString Desc = FString::Printf(TEXT("Objects in channel %s: "), *ChannelName.ToString());
    for (const AActor* A : Actors)
    {
        if (A)
        {
            Desc += A->GetName();
            Desc += TEXT(", ");
        }
    }
    Desc.RemoveFromEnd(TEXT(", "));
    return Desc;
}
