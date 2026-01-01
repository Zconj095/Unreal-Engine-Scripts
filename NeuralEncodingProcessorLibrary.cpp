#include "NeuralEncodingProcessorLibrary.h"

#include "Math/UnrealMathUtility.h"

void UNeuralEncodingProcessorLibrary::DifferenceBasedCoding(const TArray<float>& Stimuli1, const TArray<float>& Stimuli2, TArray<float>& OutDifferences)
{
    const int32 N = FMath::Min(Stimuli1.Num(), Stimuli2.Num());
    OutDifferences.SetNum(N);
    for (int32 i = 0; i < N; ++i)
    {
        OutDifferences[i] = Stimuli2[i] - Stimuli1[i];
    }
}

void UNeuralEncodingProcessorLibrary::StatisticalEncoding(const TArray<float>& InValues, TArray<float>& OutValues)
{
    const int32 N = InValues.Num();
    OutValues.SetNum(N);
    if (N == 0)
    {
        return;
    }

    double Sum = 0.0;
    for (float V : InValues) { Sum += V; }
    const double Mean = Sum / static_cast<double>(N);

    double Var = 0.0;
    for (float V : InValues)
    {
        const double D = static_cast<double>(V) - Mean;
        Var += D * D;
    }
    Var /= static_cast<double>(N);
    const double Std = FMath::Sqrt(static_cast<float>(Var));

    if (Std <= KINDA_SMALL_NUMBER)
    {
        for (int32 i = 0; i < N; ++i) { OutValues[i] = 0.0f; }
    }
    else
    {
        for (int32 i = 0; i < N; ++i)
        {
            OutValues[i] = static_cast<float>((static_cast<double>(InValues[i]) - Mean) / Std);
        }
    }
}

void UNeuralEncodingProcessorLibrary::RelationallyDeterminedProcess(const TArray<float>& InValues, TArray<float>& OutValues)
{
    const int32 N = InValues.Num();
    OutValues.SetNum(N);
    if (N == 0)
    {
        return;
    }

    OutValues[0] = 0.0f; // no previous neighbor
    for (int32 i = 1; i < N; ++i)
    {
        OutValues[i] = InValues[i] * InValues[i - 1];
    }
}

void UNeuralEncodingProcessorLibrary::PhysicallyBasedEncoding(const TArray<float>& Stimulus, TArray<float>& OutValues)
{
    const int32 N = Stimulus.Num();
    OutValues.SetNum(N);
    for (int32 i = 0; i < N; ++i)
    {
        OutValues[i] = FMath::Sin(Stimulus[i]);
    }
}

