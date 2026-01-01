#include "LLMQuantumState.h"

#include "Math/UnrealMathUtility.h"

void ULLMQuantumState::Initialize(int32 Size)
{
    const int32 N = FMath::Max(0, Size);
    Amplitudes.SetNumUninitialized(N);
    InitializeRandomState();
}

void ULLMQuantumState::InitializeRandomState()
{
    double MagnitudeSum = 0.0;

    for (int32 i = 0; i < Amplitudes.Num(); ++i)
    {
        const float Real = FMath::FRand();
        const float Imag = FMath::FRand();
        Amplitudes[i].Real = Real;
        Amplitudes[i].Imag = Imag;
        MagnitudeSum += static_cast<double>(Real) * static_cast<double>(Real)
                      + static_cast<double>(Imag) * static_cast<double>(Imag);
    }

    Normalize();
}

void ULLMQuantumState::Normalize()
{
    if (Amplitudes.Num() == 0)
    {
        return;
    }

    double MagnitudeSum = 0.0;
    for (const FComplexAmplitude& A : Amplitudes)
    {
        MagnitudeSum += static_cast<double>(A.Real) * static_cast<double>(A.Real)
                      + static_cast<double>(A.Imag) * static_cast<double>(A.Imag);
    }

    const double Norm = FMath::Sqrt(MagnitudeSum);
    const float Scalar = (Norm > 0.0) ? static_cast<float>(Norm) : 1.f;
    if (Scalar != 0.f)
    {
        for (FComplexAmplitude& A : Amplitudes)
        {
            A.DivideByScalar(Scalar);
        }
    }
}

void ULLMQuantumState::PrintState() const
{
    for (int32 i = 0; i < Amplitudes.Num(); ++i)
    {
        const FComplexAmplitude& A = Amplitudes[i];
        UE_LOG(LogTemp, Log, TEXT("State %d: (%f, %f)i"), i, A.Real, A.Imag);
    }
}

