#include "LLMGradientAscentLibrary.h"

#include "Math/UnrealMathUtility.h"

void ULLMGradientAscentLibrary::ApplyGradientAscent(const TArray<float>& Parameters, const TArray<float>& Gradients, float LearningRate, TArray<float>& OutUpdated)
{
    const int32 N = Parameters.Num();
    OutUpdated.Reset();
    if (N == 0 || Gradients.Num() != N)
    {
        return;
    }
    OutUpdated.SetNumUninitialized(N);

    const float T = FMath::Clamp(LearningRate, 0.f, 1.f);
    for (int32 i = 0; i < N; ++i)
    {
        const float Target = Parameters[i] + Gradients[i];
        OutUpdated[i] = FMath::Lerp(Parameters[i], Target, T);
    }
}

float ULLMGradientAscentLibrary::Sigmoid(float X)
{
    // Guard against overflow in exp(-x) for large negative x
    const float Clamped = FMath::Clamp(X, -60.f, 60.f);
    return 1.0f / (1.0f + FMath::Exp(-Clamped));
}

