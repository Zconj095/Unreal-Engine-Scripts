#include "CognitiveResonance.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY(LogCognitiveResonance);

UCognitiveResonance::UCognitiveResonance()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCognitiveResonance::BeginPlay()
{
    Super::BeginPlay();

    StartTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
    SimulateCognitiveResonance();
}

void UCognitiveResonance::SimulateCognitiveResonance()
{
    if (bHasSimulated || !GetOwner())
    {
        return;
    }

    bHasSimulated = true;

    const int32 SampleCount = FMath::Max(2, FMath::CeilToInt(Resolution * SimulationDuration));
    const FVector BaseLocation = GetOwner()->GetActorLocation();
    FVector PreviousPoint = BaseLocation;

    const float TimeStep = SimulationDuration / static_cast<float>(SampleCount - 1);
    const float HorizontalScale = 100.f;
    const float VerticalScale = 100.f;

    for (int32 Index = 0; Index < SampleCount; ++Index)
    {
        const float SampleTime = Index * TimeStep;
        const float Resonance = R0 + DeltaR * FMath::Cos(OmegaJ * SampleTime + Phi);

        const FVector CurrentPoint = BaseLocation + FVector(SampleTime * HorizontalScale, Resonance * VerticalScale, 0.f);

        if (Index > 0)
        {
            DrawDebugLine(GetWorld(), PreviousPoint, CurrentPoint, FColor::Green, false, 0.1f, 0, 2.f);
        }

        PreviousPoint = CurrentPoint;
    }

    UE_LOG(LogCognitiveResonance, Log, TEXT("Plotted cognitive resonance for %f seconds (%d samples)."), SimulationDuration, SampleCount);
}
