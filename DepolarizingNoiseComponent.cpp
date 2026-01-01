// Copyright Epic Games, Inc.

#include "DepolarizingNoiseComponent.h"

UDepolarizingNoiseComponent::UDepolarizingNoiseComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UDepolarizingNoiseComponent::ApplyNoise(UPARAM(ref) TArray<FComplexNumber>& StateVector)
{
    if (StateVector.Num() == 0)
    {
        return;
    }

    const double Roll = FMath::FRand();
    if (Roll >= Probability)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Applying Depolarizing Noise..."));
    for (FComplexNumber& Amp : StateVector)
    {
        Amp.Real *= Decay;
        Amp.Imaginary *= Decay;
    }
}
