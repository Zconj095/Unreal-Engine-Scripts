#include "NeuralLayer.h"

void UNeuralLayer::Initialize(int32 InLayerID, int32 NumNeurons)
{
	LayerID = InLayerID;
	RandomStream.Initialize(FPlatformTime::Cycles());
	NeuronWeights.Reset();
	NeuronWeights.Reserve(NumNeurons);
	InitializeWeights(NumNeurons);
}

void UNeuralLayer::UpdateWeights()
{
	for (int32 Index = 0; Index < NeuronWeights.Num(); ++Index)
	{
		NeuronWeights[Index] += RandomStream.FRandRange(-0.01f, 0.01f);
	}
}

const TArray<float>& UNeuralLayer::GetWeights() const
{
	return NeuronWeights;
}

void UNeuralLayer::SetWeights(const TArray<float>& Weights)
{
	NeuronWeights = Weights;
}

void UNeuralLayer::InitializeWeights(int32 NumNeurons)
{
	for (int32 Index = 0; Index < NumNeurons; ++Index)
	{
		NeuronWeights.Add(RandomStream.FRandRange(-1.f, 1.f));
	}
}
