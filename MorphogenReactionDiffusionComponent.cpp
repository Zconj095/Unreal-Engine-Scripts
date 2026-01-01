#include "MorphogenReactionDiffusionComponent.h"

#include "Math/UnrealMathUtility.h"

UMorphogenReactionDiffusionComponent::UMorphogenReactionDiffusionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UMorphogenReactionDiffusionComponent::Simulate(int32 Timesteps)
{
    const int32 N = FMath::Max(0, Size);
    const int32 T = FMath::Max(0, Timesteps);

    OutputRows = N;
    OutputCols = T;
    Output.SetNum(N * T);

    if (N == 0 || T == 0)
    {
        return;
    }

    // Initialize u and v
    TArray<float> U; U.Init(0.f, N);
    TArray<float> V; V.Init(0.1f, N);

    const int32 Half = N / 2;
    for (int32 i = 0; i < N; ++i)
    {
        U[i] = (i < Half) ? -1.0f : 1.0f;
        // First column of output = u initial
        const int32 Index = i * T + 0;
        Output[Index] = U[i];
    }

    // Time stepping
    for (int32 t = 1; t < T; ++t)
    {
        for (int32 i = 0; i < N; ++i)
        {
            float DuDt = 0.f, DvDt = 0.f;
            // Using previous u value from output, and current V[i]
            const float UPrev = Output[i * T + (t - 1)];
            FitzHughNagumo(UPrev, V[i], static_cast<float>(t - 1), DuDt, DvDt);

            const float UNext = UPrev + DuDt * Dt;
            V[i] = V[i] + DvDt * Dt;

            Output[i * T + t] = UNext;
        }
    }
}

void UMorphogenReactionDiffusionComponent::FitzHughNagumo(float UVal, float VVal, float Time, float& OutDuDt, float& OutDvDt) const
{
    const float Du = 1.0f;
    const float Dv = 0.0f;
    const float F = UVal - FMath::Pow(UVal, 3) - VVal;
    const float G = UVal + A;

    // Placeholder spatial derivative terms set to zero (no diffusion discretization here)
    OutDuDt = Du * 0.0f + F;
    OutDvDt = Dv * 0.0f + G;
}

