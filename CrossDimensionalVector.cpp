#include "CrossDimensionalVector.h"

#include "Math/UnrealMathUtility.h"

void UCrossDimensionalVector::Initialize(int32 Dimensions, int32 Seed, bool bDeterministic)
{
    const int32 N = FMath::Max(0, Dimensions);
    HyperVector.SetNumUninitialized(N);
    QuantumVector.SetNumUninitialized(N);

    FRandomStream Rng;
    if (bDeterministic)
    {
        Rng.Initialize(Seed);
    }
    else
    {
        Rng.Initialize(FMath::Rand());
    }

    for (int32 i = 0; i < N; ++i)
    {
        // Hyperdimensional: [-1,1], Quantum: [0,1]
        HyperVector[i] = Rng.GetFraction() * 2.0f - 1.0f;
        QuantumVector[i] = Rng.GetFraction();
    }
}

void UCrossDimensionalVector::CrossReference(ECorrelationMode Mode, TArray<float>& OutResult) const
{
    const int32 N = FMath::Min(HyperVector.Num(), QuantumVector.Num());
    OutResult.SetNumUninitialized(N);
    for (int32 i = 0; i < N; ++i)
    {
        const float H = HyperVector[i];
        const float Q = QuantumVector[i];
        switch (Mode)
        {
        case ECorrelationMode::Add:      OutResult[i] = H + Q; break;
        case ECorrelationMode::Subtract: OutResult[i] = H - Q; break;
        case ECorrelationMode::Multiply: OutResult[i] = H * Q; break;
        case ECorrelationMode::Average:  OutResult[i] = 0.5f * (H + Q); break;
        case ECorrelationMode::Min:      OutResult[i] = FMath::Min(H, Q); break;
        case ECorrelationMode::Max:      OutResult[i] = FMath::Max(H, Q); break;
        }
    }
}

void UCrossDimensionalVector::CrossReferenceCustom(FFloatCorrelation Correlation, TArray<float>& OutResult) const
{
    const int32 N = FMath::Min(HyperVector.Num(), QuantumVector.Num());
    OutResult.SetNumUninitialized(N);
    if (!Correlation.IsBound())
    {
        for (int32 i = 0; i < N; ++i)
        {
            OutResult[i] = 0.0f;
        }
        return;
    }
    for (int32 i = 0; i < N; ++i)
    {
        OutResult[i] = Correlation.Execute(HyperVector[i], QuantumVector[i]);
    }
}

UCrossDimensionalVector* UCrossVectorAllocatorLibrary::CreateCrossDimensionalVector(int32 Dimensions, int32 Seed, bool bDeterministic, UObject* Outer)
{
    UObject* UseOuter = Outer ? Outer : GetTransientPackage();
    UCrossDimensionalVector* Obj = NewObject<UCrossDimensionalVector>(UseOuter);
    if (Obj)
    {
        Obj->Initialize(Dimensions, Seed, bDeterministic);
    }
    return Obj;
}

