#include "NeuralCodingProcessorLibrary.h"

void UNeuralCodingProcessorLibrary::DifferenceBasedCoding(const TArray<float>& Stimuli1, const TArray<float>& Stimuli2, TArray<float>& OutDifferences)
{
    const int32 N = FMath::Min(Stimuli1.Num(), Stimuli2.Num());
    OutDifferences.SetNum(N);
    for (int32 i = 0; i < N; ++i)
    {
        OutDifferences[i] = Stimuli2[i] - Stimuli1[i];
    }
}

void UNeuralCodingProcessorLibrary::NeuralActivity(const TArray<float>& Differences, const TArray<float>& Stimulus, TArray<float>& OutActivity)
{
    const int32 N = FMath::Min(Differences.Num(), Stimulus.Num());
    OutActivity.SetNum(N);
    for (int32 i = 0; i < N; ++i)
    {
        OutActivity[i] = Differences[i] + Stimulus[i];
    }
}

