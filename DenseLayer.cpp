#include "DenseLayer.h"

#include <cmath>

FDenseLayer::FDenseLayer(int32 InInputSize, int32 InOutputSize)
    : InputSize(InInputSize)
    , OutputSize(InOutputSize)
{
    Weights.SetNumZeroed(InputSize * OutputSize);
    Biases.SetNumZeroed(OutputSize);
    InitializeWeights();
}

void FDenseLayer::InitializeWeights()
{
    // Initialize weights uniformly in [-1, 1], biases to 0
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

TArray<float> FDenseLayer::Forward(const TArray<float>& Input) const
{
    return FDenseLayer::Forward(Input, Weights, Biases, InputSize, OutputSize);
}

TArray<float> FDenseLayer::Forward(const TArray<float>& Input,
                                   const TArray<float>& InWeights,
                                   const TArray<float>& InBiases,
                                   int32 InInputSize,
                                   int32 InOutputSize)
{
    TArray<float> Output;
    Output.SetNumZeroed(InOutputSize);

    // Basic shape safety (optional): ensure sizes are consistent
    if (Input.Num() < InInputSize || InWeights.Num() < InInputSize * InOutputSize || InBiases.Num() < InOutputSize)
    {
        // Return zeroed output if shapes don't align
        return Output;
    }

    for (int32 j = 0; j < InOutputSize; ++j)
    {
        float Sum = InBiases[j];
        for (int32 i = 0; i < InInputSize; ++i)
        {
            const int32 WIndex = i * InOutputSize + j; // row-major [i, j]
            Sum += Input[i] * InWeights[WIndex];
        }
        // Hyperbolic tangent activation
        Output[j] = static_cast<float>(std::tanh(static_cast<double>(Sum)));
    }

    return Output;
}

