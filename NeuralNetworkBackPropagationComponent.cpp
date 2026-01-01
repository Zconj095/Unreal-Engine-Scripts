#include "NeuralNetworkBackPropagationComponent.h"

#include "Logging/LogMacros.h"

UNeuralNetworkBackPropagationComponent::UNeuralNetworkBackPropagationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralNetworkBackPropagationComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeNetwork();
	TrainNetwork(10000);
}

void UNeuralNetworkBackPropagationComponent::InitializeNetwork()
{
	NumInputNodes = FMath::Max(1, NumInputNodes);
	NumHiddenNodes = FMath::Max(1, NumHiddenNodes);
	NumOutputNodes = FMath::Max(1, NumOutputNodes);

	InputHiddenWeights.SetNum(NumInputNodes * NumHiddenNodes);
	HiddenBiases.SetNum(NumHiddenNodes);
	HiddenOutputWeights.SetNum(NumHiddenNodes * NumOutputNodes);
	OutputBiases.SetNum(NumOutputNodes);

	for (int32 i = 0; i < NumInputNodes; ++i)
	{
		for (int32 j = 0; j < NumHiddenNodes; ++j)
		{
			InputHiddenWeights[IdxIH(i, j)] = FMath::FRandRange(-0.5f, 0.5f);
		}
	}

	for (int32 i = 0; i < NumHiddenNodes; ++i)
	{
		HiddenBiases[i] = FMath::FRandRange(-0.5f, 0.5f);
	}

	for (int32 i = 0; i < NumHiddenNodes; ++i)
	{
		for (int32 j = 0; j < NumOutputNodes; ++j)
		{
			HiddenOutputWeights[IdxHO(i, j)] = FMath::FRandRange(-0.5f, 0.5f);
		}
	}

	for (int32 i = 0; i < NumOutputNodes; ++i)
	{
		OutputBiases[i] = FMath::FRandRange(-0.5f, 0.5f);
	}
}

float UNeuralNetworkBackPropagationComponent::Sigmoid(float X) const
{
	return 1.f / (1.f + FMath::Exp(-X));
}

float UNeuralNetworkBackPropagationComponent::SigmoidDerivative(float X) const
{
	return X * (1.f - X);
}

void UNeuralNetworkBackPropagationComponent::TrainNetwork(int32 Epochs)
{
	if (NumInputNodes != 3 || NumOutputNodes != 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Demo training assumes 3 inputs and 1 output (XOR). Current config: %d inputs, %d outputs."), NumInputNodes, NumOutputNodes);
	}

	// Dummy training data (XOR)
	const float Inputs[4][3] = {
		{0.f, 0.f, 1.f},
		{1.f, 0.f, 1.f},
		{0.f, 1.f, 1.f},
		{1.f, 1.f, 1.f}
	};
	const float Targets[4] = {0.f, 1.f, 1.f, 0.f};

	TArray<float> HiddenLayer; HiddenLayer.SetNumZeroed(NumHiddenNodes);
	TArray<float> OutputLayer; OutputLayer.SetNumZeroed(NumOutputNodes);
	TArray<float> OutputErrors; OutputErrors.SetNumZeroed(NumOutputNodes);
	TArray<float> OutputDeltas; OutputDeltas.SetNumZeroed(NumOutputNodes);
	TArray<float> HiddenDeltas; HiddenDeltas.SetNumZeroed(NumHiddenNodes);

	for (int32 Epoch = 0; Epoch < Epochs; ++Epoch)
	{
		for (int32 Sample = 0; Sample < 4; ++Sample)
		{
			// Forward pass: hidden
			for (int32 h = 0; h < NumHiddenNodes; ++h)
			{
				float Sum = HiddenBiases[h];
				for (int32 i = 0; i < NumInputNodes; ++i)
				{
					Sum += Inputs[Sample][i] * InputHiddenWeights[IdxIH(i, h)];
				}
				HiddenLayer[h] = Sigmoid(Sum);
			}

			// Forward pass: output
			for (int32 o = 0; o < NumOutputNodes; ++o)
			{
				float Sum = OutputBiases[o];
				for (int32 h = 0; h < NumHiddenNodes; ++h)
				{
					Sum += HiddenLayer[h] * HiddenOutputWeights[IdxHO(h, o)];
				}
				OutputLayer[o] = Sigmoid(Sum);
			}

			// Error
			for (int32 o = 0; o < NumOutputNodes; ++o)
			{
				OutputErrors[o] = Targets[Sample] - OutputLayer[o];
				OutputDeltas[o] = OutputErrors[o] * SigmoidDerivative(OutputLayer[o]);
			}

			// Hidden deltas
			for (int32 h = 0; h < NumHiddenNodes; ++h)
			{
				float ErrSum = 0.f;
				for (int32 o = 0; o < NumOutputNodes; ++o)
				{
					ErrSum += OutputDeltas[o] * HiddenOutputWeights[IdxHO(h, o)];
				}
				HiddenDeltas[h] = ErrSum * SigmoidDerivative(HiddenLayer[h]);
			}

			// Update output weights/biases
			for (int32 o = 0; o < NumOutputNodes; ++o)
			{
				for (int32 h = 0; h < NumHiddenNodes; ++h)
				{
					HiddenOutputWeights[IdxHO(h, o)] += LearningRate * HiddenLayer[h] * OutputDeltas[o];
				}
				OutputBiases[o] += LearningRate * OutputDeltas[o];
			}

			// Update input-hidden weights/biases
			for (int32 h = 0; h < NumHiddenNodes; ++h)
			{
				for (int32 i = 0; i < NumInputNodes; ++i)
				{
					HiddenBiases[h] += LearningRate * HiddenDeltas[h];
					InputHiddenWeights[IdxIH(i, h)] += LearningRate * Inputs[Sample][i] * HiddenDeltas[h];
				}
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Training complete!"));
}
