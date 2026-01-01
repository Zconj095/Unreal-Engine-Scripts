#include "NeuralAutoencoderComponent.h"
#include "Math/UnrealMathUtility.h"

UNeuralAutoencoderComponent::UNeuralAutoencoderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralAutoencoderComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeNetwork();
	TrainModel();
}

void UNeuralAutoencoderComponent::InitializeNetwork()
{
	auto RandomVector = [](int32 Num) {
		TArray<float> Vec;
		Vec.SetNumZeroed(Num);
		for (int32 i = 0; i < Num; ++i)
		{
			Vec[i] = FMath::FRandRange(-0.1f, 0.1f);
		}
		return Vec;
	};

	WeightsInToHidden = RandomVector(InputDimension * HiddenDimension);
	WeightsHiddenToOut = RandomVector(HiddenDimension * InputDimension);

	HiddenNodes.SetNumZeroed(HiddenDimension);
	OutputNodes.SetNumZeroed(InputDimension);
	InputNodes.SetNumZeroed(InputDimension);

	for (int32 i = 0; i < InputDimension; ++i)
	{
		InputNodes[i] = FMath::FRandRange(0.0f, 1.0f);
	}
}

void UNeuralAutoencoderComponent::TrainModel()
{
	for (int32 Epoch = 0; Epoch < Epochs; ++Epoch)
	{
		ForwardStep();
		const float Error = ComputeError();
		BackwardStep();
		if (Epoch % 100 == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Epoch %d, Loss = %.6f"), Epoch, Error);
		}
	}
}

void UNeuralAutoencoderComponent::ForwardStep()
{
	for (int32 j = 0; j < HiddenDimension; ++j)
	{
		float Sum = 0.0f;
		for (int32 i = 0; i < InputDimension; ++i)
		{
			Sum += InputNodes[i] * WeightsInToHidden[i * HiddenDimension + j];
		}
		HiddenNodes[j] = Sigmoid(Sum);
	}

	for (int32 i = 0; i < InputDimension; ++i)
	{
		float Sum = 0.0f;
		for (int32 j = 0; j < HiddenDimension; ++j)
		{
			Sum += HiddenNodes[j] * WeightsHiddenToOut[j * InputDimension + i];
		}
		OutputNodes[i] = Sigmoid(Sum);
	}
}

float UNeuralAutoencoderComponent::ComputeError() const
{
	float ReconstructionLoss = 0.0f;
	for (int32 i = 0; i < InputDimension; ++i)
	{
		const float Diff = InputNodes[i] - OutputNodes[i];
		ReconstructionLoss += Diff * Diff;
	}

	float RegularizationLoss = 0.0f;
	for (float Weight : WeightsInToHidden)
	{
		RegularizationLoss += Weight * Weight;
	}

	return ReconstructionLoss + Lambda * RegularizationLoss;
}

void UNeuralAutoencoderComponent::BackwardStep()
{
	TArray<float> OutputGradients;
	OutputGradients.SetNumZeroed(InputDimension);
	for (int32 i = 0; i < InputDimension; ++i)
	{
		const float Error = OutputNodes[i] - InputNodes[i];
		OutputGradients[i] = Error * SigmoidDerivative(OutputNodes[i]);
	}

	TArray<float> HiddenGradients;
	HiddenGradients.SetNumZeroed(HiddenDimension);
	for (int32 j = 0; j < HiddenDimension; ++j)
	{
		float Error = 0.0f;
		for (int32 i = 0; i < InputDimension; ++i)
		{
			Error += OutputGradients[i] * WeightsHiddenToOut[j * InputDimension + i];
		}
		HiddenGradients[j] = Error * SigmoidDerivative(HiddenNodes[j]);
	}

	for (int32 j = 0; j < HiddenDimension; ++j)
	{
		for (int32 i = 0; i < InputDimension; ++i)
		{
			const int32 IndexH = j * InputDimension + i;
			WeightsHiddenToOut[IndexH] -= LearningRate * OutputGradients[i] * HiddenNodes[j];
		}
	}

	for (int32 i = 0; i < InputDimension; ++i)
	{
		for (int32 j = 0; j < HiddenDimension; ++j)
		{
			const int32 IndexI = i * HiddenDimension + j;
			WeightsInToHidden[IndexI] -= LearningRate * HiddenGradients[j] * InputNodes[i];
		}
	}
}

float UNeuralAutoencoderComponent::Sigmoid(float Value) const
{
	return 1.0f / (1.0f + FMath::Exp(-Value));
}

float UNeuralAutoencoderComponent::SigmoidDerivative(float Value) const
{
	return Value * (1.0f - Value);
}
