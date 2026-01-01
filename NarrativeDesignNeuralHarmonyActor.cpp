#include "NarrativeDesignNeuralHarmonyActor.h"

ANarrativeDesignNeuralHarmonyActor::ANarrativeDesignNeuralHarmonyActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ANarrativeDesignNeuralHarmonyActor::BeginPlay()
{
    Super::BeginPlay();

    NDNH = CalculateNDNH();
    UE_LOG(LogTemp, Log, TEXT("Initial NDNH: %.4f"), NDNH);
}

void ANarrativeDesignNeuralHarmonyActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    NDNH = CalculateNDNH();
    UE_LOG(LogTemp, Log, TEXT("NDNH: %.4f"), NDNH);
}

void ANarrativeDesignNeuralHarmonyActor::UpdateStorytellingNeuralActivity(const TArray<float>& NewNeuralActivity)
{
    StorytellingNeuralActivity = NewNeuralActivity;
}

void ANarrativeDesignNeuralHarmonyActor::UpdateNarrativeFrequencies(const TArray<float>& NewNarrativeFrequencies)
{
    NarrativeFrequencies = NewNarrativeFrequencies;
}

float ANarrativeDesignNeuralHarmonyActor::GetNDNH() const
{
    return NDNH;
}

float ANarrativeDesignNeuralHarmonyActor::CalculateNDNH() const
{
    if (StorytellingNeuralActivity.Num() != NarrativeFrequencies.Num())
    {
        UE_LOG(LogTemp, Error, TEXT("Storytelling activity and narrative frequency arrays must match in size."));
        return 0.f;
    }

    const int32 Count = StorytellingNeuralActivity.Num();
    if (Count == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No narrative arcs available for NDNH calculation."));
        return 0.f;
    }

    double Sum = 0.0;
    for (int32 Index = 0; Index < Count; ++Index)
    {
        Sum += StorytellingNeuralActivity[Index] * NarrativeFrequencies[Index];
    }

    return static_cast<float>(Sum / Count);
}
