// Copyright Epic Games, Inc.

#include "AerSimulator.h"
#include "Math/UnrealMathUtility.h"

TMap<FString, float> UAerSimulator::SimulateCircuit(const TArray<FString>& Circuit, int32 NumQubits) const
{
    // Simplified simulation: create random probabilities for each basis state (like the Unity version).
    // Clamp qubits so we don't create an excessively large map.
    const int32 ClampedQubits = FMath::Clamp(NumQubits, 1, 10); // up to 1024 states
    const int32 NumStates = 1 << ClampedQubits;

    TMap<FString, float> StateProbabilities;
    StateProbabilities.Reserve(NumStates);

    float Total = 0.0f;

    for (int32 StateIndex = 0; StateIndex < NumStates; ++StateIndex)
    {
        // Build binary state string padded to qubit count.
        FString StateString;
        StateString.Reserve(ClampedQubits);
        for (int32 Bit = ClampedQubits - 1; Bit >= 0; --Bit)
        {
            const bool bIsOne = (StateIndex >> Bit) & 1;
            StateString.AppendChar(bIsOne ? TEXT('1') : TEXT('0'));
        }

        const float Prob = FMath::FRandRange(0.0f, 1.0f);
        StateProbabilities.Add(StateString, Prob);
        Total += Prob;
    }

    // Normalize probabilities so they sum to 1.0.
    if (Total > KINDA_SMALL_NUMBER)
    {
        for (TPair<FString, float>& Pair : StateProbabilities)
        {
            Pair.Value /= Total;
        }
    }
    else
    {
        // Fallback: evenly distribute if random values were degenerate.
        const float EvenProb = 1.0f / static_cast<float>(NumStates);
        for (TPair<FString, float>& Pair : StateProbabilities)
        {
            Pair.Value = EvenProb;
        }
    }

    return StateProbabilities;
}
