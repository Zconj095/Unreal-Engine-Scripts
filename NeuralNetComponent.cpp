#include "NeuralNetComponent.h"
#include "Math/UnrealMathUtility.h"

UNeuralNetComponent::UNeuralNetComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralNetComponent::OnRegister()
{
    Super::OnRegister();
    InitializeNetwork();
}

int32 UNeuralNetComponent::GetWeightIndex(int32 LayerIndex, int32 Source, int32 Target) const
{
    if (!WeightMatrices.IsValidIndex(LayerIndex))
    {
        return INDEX_NONE;
    }
    const int32 NumTargets = LayerSizes[LayerIndex + 1];
    return Source * NumTargets + Target;
}

float UNeuralNetComponent::ComputeActivation(float Sum) const
{
    return FMath::Tanh(Sum);
}

void UNeuralNetComponent::InitializeNetwork()
{
    Layers.Empty();
    WeightMatrices.Empty();

    if (LayerSizes.Num() < 2)
    {
        UE_LOG(LogTemp, Warning, TEXT("NeuralNetComponent requires at least two layers."));
        bInitialized = false;
        return;
    }

    for (int32 Size : LayerSizes)
    {
        Layers.AddDefaulted();
        Layers.Last().SetNumZeroed(Size);
    }

    for (int32 Index = 0; Index < LayerSizes.Num() - 1; ++Index)
    {
        const int32 SourceSize = LayerSizes[Index];
        const int32 TargetSize = LayerSizes[Index + 1];
        WeightMatrices.AddDefaulted();
        WeightMatrices.Last().SetNumZeroed(SourceSize * TargetSize);

        for (int32 Source = 0; Source < SourceSize; ++Source)
        {
            for (int32 TargetInt = 0; TargetInt < TargetSize; ++TargetInt)
            {
                const int32 WeightIdx = Source * TargetSize + TargetInt;
                WeightMatrices.Last()[WeightIdx] = FMath::FRandRange(InitialWeightRange.X, InitialWeightRange.Y);
            }
        }
    }

    bInitialized = true;
}

bool UNeuralNetComponent::FeedForward(const TArray<float>& Input, TArray<float>& Output)
{
    if (!bInitialized || Input.Num() != LayerSizes[0])
    {
        return false;
    }

    Layers[0] = Input;

    for (int32 LayerIdx = 0; LayerIdx < WeightMatrices.Num(); ++LayerIdx)
    {
        const int32 SourceSize = LayerSizes[LayerIdx];
        const int32 TargetSize = LayerSizes[LayerIdx + 1];
        for (int32 TargetInt = 0; TargetInt < TargetSize; ++TargetInt)
        {
            float Sum = 0.0f;
            for (int32 Source = 0; Source < SourceSize; ++Source)
            {
                const int32 WeightIndex = Source * TargetSize + TargetInt;
                Sum += Layers[LayerIdx][Source] * WeightMatrices[LayerIdx][WeightIndex];
            }
            Layers[LayerIdx + 1][TargetInt] = ComputeActivation(Sum);
        }
    }

    Output = Layers.Last();
    return true;
}
