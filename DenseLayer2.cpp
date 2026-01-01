#include "DenseLayer2.h"

#include <cmath>

FDenseLayer2::FDenseLayer2(int32 InInputSize, int32 InOutputSize)
    : InputSize(InInputSize)
    , OutputSize(InOutputSize)
{
    Weights.SetNumZeroed(InputSize * OutputSize);
    Biases.SetNumZeroed(OutputSize);
    InitializeWeights();
}

void FDenseLayer2::InitializeWeights()
{
    for (int32 i = 0; i < InputSize; ++i)
    {
        for (int32 j = 0; j < OutputSize; ++j)
        {
            const int32 Index = i * OutputSize + j;
            Weights[Index] = FMath::FRandRange(-1.f, 1.f);
        }
    }

    for (int32 j = 0; j < OutputSize; ++j)
    {
        Biases[j] = 0.f;
    }
}

TArray<float> FDenseLayer2::Forward(const TArray<float>& Input) const
{
    TArray<float> Output;
    Output.SetNumZeroed(OutputSize);

    if (Input.Num() < InputSize)
    {
        return Output; // shape guard
    }

    for (int32 j = 0; j < OutputSize; ++j)
    {
        float Sum = Biases[j];
        for (int32 i = 0; i < InputSize; ++i)
        {
            const int32 WIndex = i * OutputSize + j; // [i, j]
            Sum += Input[i] * Weights[WIndex];
        }
        Output[j] = static_cast<float>(std::tanh(static_cast<double>(Sum)));
    }

    return Output;
}

