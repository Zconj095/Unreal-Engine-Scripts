#include "LLMQuantumState2.h"

#include "Math/UnrealMathUtility.h"

void ULLMQuantumState2::Initialize(int32 Dimensions)
{
    Amplitudes.SetNumUninitialized(FMath::Max(0, Dimensions));
    Randomize();
}

void ULLMQuantumState2::Randomize()
{
    const int32 N = Amplitudes.Num();
    for (int32 i = 0; i < N; ++i)
    {
        Amplitudes[i] = FMath::FRand(); // [0,1)
    }
    Normalize();
}

void ULLMQuantumState2::Normalize()
{
    double SumSq = 0.0;
    for (float V : Amplitudes) { SumSq += static_cast<double>(V) * static_cast<double>(V); }
    const double Norm = FMath::Sqrt(SumSq);
    const float S = (Norm > 0.0) ? static_cast<float>(Norm) : 1.0f;
    if (S != 0.0f)
    {
        for (float& V : Amplitudes) { V /= S; }
    }
}

ULLMQuantumState2* ULLMQuantumState2::Interfere(const ULLMQuantumState2* Other) const
{
    if (!Other)
    {
        return nullptr;
    }
    const int32 N = FMath::Min(Amplitudes.Num(), Other->Amplitudes.Num());
    UObject* Outer = GetTransientPackage();
    ULLMQuantumState2* NewState = NewObject<ULLMQuantumState2>(Outer);
    if (!NewState)
    {
        return nullptr;
    }
    NewState->Amplitudes.SetNumUninitialized(N);
    for (int32 i = 0; i < N; ++i)
    {
        NewState->Amplitudes[i] = 0.5f * (Amplitudes[i] + Other->Amplitudes[i]);
    }
    return NewState;
}

float ULLMQuantumState2::Measure() const
{
    double SumSq = 0.0;
    for (float V : Amplitudes) { SumSq += static_cast<double>(V) * static_cast<double>(V); }
    return static_cast<float>(FMath::Sqrt(SumSq));
}

