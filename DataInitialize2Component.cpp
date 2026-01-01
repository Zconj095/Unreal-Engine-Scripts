#include "DataInitialize2Component.h"

#include "Misc/OutputDevice.h"

UDataInitialize2Component::UDataInitialize2Component()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDataInitialize2Component::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("DataInitialize2Component initialized on %s"), *GetOwner()->GetName());
}

FString UDataInitialize2Component::ArrayToString(const TArray<float>& Array) const
{
    TArray<FString> Parts;
    Parts.Reserve(Array.Num());
    for (float V : Array)
    {
        Parts.Add(LexToString(V));
    }
    return FString::Join(Parts, TEXT(","));
}

