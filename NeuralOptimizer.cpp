#include "NeuralOptimizer.h"

#include "Math/UnrealMathUtility.h"

void UNeuralOptimizer::Initialize(int32 InInputSize, int32 InOutputSize)
{
    InputSize = FMath::Max(0, InInputSize);
    OutputSize = FMath::Max(0, InOutputSize);

    const int32 Count = InputSize * OutputSize;
    Weights.SetNumUninitialized(Count);
    Biases.SetNumUninitialized(OutputSize);

    // Randomize weights in [-1,1], zero biases
    for (int32 idx = 0; idx < Count; ++idx)
    {
        Weights[idx] = FMath::FRandRange(-1.f, 1.f);
    }
    for (int32 j = 0; j < OutputSize; ++j)
    {
        Biases[j] = 0.f;
    }
}

void UNeuralOptimizer::AdjustWeights(const TArray<float>& Gradients, float LearningRate)
{
    if (InputSize <= 0 || OutputSize <= 0) { return; }
    if (Gradients.Num() < OutputSize) { return; }

    const float LR = LearningRate;
    for (int32 i = 0; i < InputSize; ++i)
    {
        const int32 RowBase = i * OutputSize;
        for (int32 j = 0; j < OutputSize; ++j)
        {
            Weights[RowBase + j] -= Gradients[j] * LR;
        }
    }
}

void UNeuralOptimizer::GetWeightsRows(TArray<FFloatArray>& OutRows) const
{
    OutRows.Reset();
    if (InputSize <= 0 || OutputSize <= 0) { return; }

    OutRows.SetNum(InputSize);
    for (int32 i = 0; i < InputSize; ++i)
    {
        FFloatArray& Row = OutRows[i];
        Row.Values.SetNumUninitialized(OutputSize);
        const int32 RowBase = i * OutputSize;
        for (int32 j = 0; j < OutputSize; ++j)
        {
            Row.Values[j] = Weights[RowBase + j];
        }
    }
}

