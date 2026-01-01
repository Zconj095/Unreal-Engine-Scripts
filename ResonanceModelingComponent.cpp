#include "ResonanceModelingComponent.h"

#include "Math/UnrealMathUtility.h"

UResonanceModelingComponent::UResonanceModelingComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UResonanceModelingComponent::BeginPlay()
{
    Super::BeginPlay();

    // Build t = linspace(0, DurationSeconds, SampleCount)
    TArray<float> T; T.SetNum(FMath::Max(0, SampleCount));
    if (SampleCount > 1)
    {
        const float Step = DurationSeconds / static_cast<float>(SampleCount - 1);
        for (int32 i = 0; i < SampleCount; ++i)
        {
            T[i] = static_cast<float>(i) * Step;
        }
    }
    else if (SampleCount == 1)
    {
        T[0] = 0.f;
    }

    // Phases: [0, PI/2]
    TArray<float> Phase; Phase.Add(0.f); Phase.Add(PI / 2.0f);

    TArray<float> F1, F2, Interaction;
    ResonantInteractions(T, Phase, F1, F2, Interaction);

    const float F1_0 = (F1.Num() > 0) ? F1[0] : 0.f;
    const float F2_0 = (F2.Num() > 0) ? F2[0] : 0.f;
    const float Int_0 = (Interaction.Num() > 0) ? Interaction[0] : 0.f;

    UE_LOG(LogTemp, Log, TEXT("F1: %f, F2: %f, Interaction: %f"), F1_0, F2_0, Int_0);
}

void UResonanceModelingComponent::ResonantInteractions(const TArray<float>& T, const TArray<float>& Phase, TArray<float>& OutF1, TArray<float>& OutF2, TArray<float>& OutInteraction) const
{
    const float Phase1 = (Phase.Num() > 0) ? Phase[0] : 0.f;
    const float Phase2 = (Phase.Num() > 1) ? Phase[1] : 0.f;

    const int32 N = T.Num();
    OutF1.SetNum(N);
    OutF2.SetNum(N);
    OutInteraction.SetNum(N);

    for (int32 i = 0; i < N; ++i)
    {
        const float x = T[i];
        const float F1v = 3.f * FMath::Sin(1.f * (x + Phase1));
        const float F2v = FMath::Sin(3.f * (x + Phase2));
        OutF1[i] = F1v;
        OutF2[i] = F2v;
        OutInteraction[i] = F1v * F2v;
    }
}

