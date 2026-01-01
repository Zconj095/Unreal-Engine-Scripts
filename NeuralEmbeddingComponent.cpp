#include "NeuralEmbeddingComponent.h"

UNeuralEmbeddingComponent::UNeuralEmbeddingComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralEmbeddingComponent::BeginPlay()
{
    Super::BeginPlay();

    Layers.Reset();
    Layers.SetNum(2);
    Layers[0].Init(InputDim, 64);
    Layers[1].Init(64, OutputDim);
}

TArray<float> UNeuralEmbeddingComponent::Forward(const TArray<float>& X) const
{
    TArray<float> Out = X;
    for (const FEmbeddingLayer& Layer : Layers)
    {
        Out = Layer.Compute(Out);
    }
    return Out;
}

