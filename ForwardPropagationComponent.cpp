#include "ForwardPropagationComponent.h"

#include "Logging/LogMacros.h"

UForwardPropagationComponent::UForwardPropagationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UForwardPropagationComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeNetwork();

	if (ExampleInput.Num() != InputLayerSize)
	{
		UE_LOG(LogTemp, Warning, TEXT("ForwardPropagationComponent: ExampleInput size (%d) does not match InputLayerSize (%d). Skipping demo run."), ExampleInput.Num(), InputLayerSize);
		return;
	}

	const TArray<float> Output = ForwardPropagate(ExampleInput);
	if (Output.Num() > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Network Output: %f"), Output[0]);
	}
}

void UForwardPropagationComponent::InitializeNetwork()
{
	InputLayerSize = FMath::Max(1, InputLayerSize);
	HiddenLayerSize = FMath::Max(1, HiddenLayerSize);
	OutputLayerSize = FMath::Max(1, OutputLayerSize);

	InitializeWeights(InputToHiddenWeights, InputLayerSize, HiddenLayerSize);
	InitializeWeights(HiddenToOutputWeights, HiddenLayerSize, OutputLayerSize);
	InitializeBiases(HiddenBiases, HiddenLayerSize);
	InitializeBiases(OutputBiases, OutputLayerSize);
}

void UForwardPropagationComponent::InitializeWeights(TArray<float>& Weights, int32 Rows, int32 Cols)
{
	Weights.SetNum(Rows * Cols);
	for (int32 r = 0; r < Rows; ++r)
	{
		for (int32 c = 0; c < Cols; ++c)
		{
			Weights[Idx(r, c, Cols)] = FMath::FRandRange(-1.0f, 1.0f);
		}
	}
}

void UForwardPropagationComponent::InitializeBiases(TArray<float>& Biases, int32 Size)
{
	Biases.SetNum(Size);
	for (int32 i = 0; i < Size; ++i)
	{
		Biases[i] = FMath::FRandRange(-1.0f, 1.0f);
	}
}

float UForwardPropagationComponent::Sigmoid(float X)
{
	return 1.0f / (1.0f + FMath::Exp(-X));
}

TArray<float> UForwardPropagationComponent::ForwardPropagate(const TArray<float>& Input)
{
	TArray<float> EmptyResult;
	if (Input.Num() != InputLayerSize)
	{
		UE_LOG(LogTemp, Error, TEXT("ForwardPropagationComponent: Input size (%d) does not match InputLayerSize (%d)."), Input.Num(), InputLayerSize);
		return EmptyResult;
	}

	// Hidden layer
	TArray<float> HiddenLayer;
	HiddenLayer.SetNumZeroed(HiddenLayerSize);
	for (int32 i = 0; i < HiddenLayerSize; ++i)
	{
		float Sum = HiddenBiases.IsValidIndex(i) ? HiddenBiases[i] : 0.0f;
		for (int32 j = 0; j < InputLayerSize; ++j)
		{
			Sum += Input[j] * InputToHiddenWeights[Idx(j, i, HiddenLayerSize)];
		}
		HiddenLayer[i] = Sigmoid(Sum);
	}

	// Output layer
	TArray<float> OutputLayer;
	OutputLayer.SetNumZeroed(OutputLayerSize);
	for (int32 i = 0; i < OutputLayerSize; ++i)
	{
		float Sum = OutputBiases.IsValidIndex(i) ? OutputBiases[i] : 0.0f;
		for (int32 j = 0; j < HiddenLayerSize; ++j)
		{
			Sum += HiddenLayer[j] * HiddenToOutputWeights[Idx(j, i, OutputLayerSize)];
		}
		OutputLayer[i] = Sigmoid(Sum);
	}

	return OutputLayer;
}
