#include "AstrocyteNetworkComponent.h"

#include "Math/UnrealMathUtility.h"

UAstrocyteNetworkComponent::UAstrocyteNetworkComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAstrocyteNetworkComponent::BeginPlay()
{
    Super::BeginPlay();

    // Initialize with 20 glial cells (equivalent to Unity's Awake)
    Astrocytes.Reset();
    Astrocytes.Reserve(20);
    for (int32 i = 0; i < 20; ++i)
    {
        Astrocytes.Add(FGlialCell());
    }
}

TArray<FGlialCell> UAstrocyteNetworkComponent::Simulate(const TArray<float>& Glutamate, int32 Timesteps)
{
    TArray<FGlialCell> Transmissions;

    if (Glutamate.Num() <= 0 || Astrocytes.Num() <= 0 || Timesteps <= 0)
    {
        return Transmissions; // empty
    }

    for (int32 t = 0; t < Timesteps; ++t)
    {
        const int32 GluIndex = FMath::RandRange(0, Glutamate.Num() - 1);
        const float Glut = Glutamate[GluIndex];

        const int32 AstroIndex = FMath::RandRange(0, Astrocytes.Num() - 1);
        FGlialCell& Astro = Astrocytes[AstroIndex];
        Astro.Uptake(Glut);

        if (Astro.Gliotransmission())
        {
            Transmissions.Add(Astro);
        }
    }

    if (Transmissions.Num() > 5)
    {
        TArray<FGlialCell> LastFive;
        const int32 Count = 5;
        LastFive.Reserve(Count);
        const int32 StartIndex = Transmissions.Num() - Count;
        for (int32 i = 0; i < Count; ++i)
        {
            LastFive.Add(Transmissions[StartIndex + i]);
        }
        return LastFive;
    }

    // If <= 5, return empty to match the Unity logic
    return TArray<FGlialCell>();
}
