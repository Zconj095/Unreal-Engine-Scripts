#include "NeuralSystemDynamicsComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UNeuralSystemDynamicsComponent::UNeuralSystemDynamicsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralSystemDynamicsComponent::BeginPlay()
{
	Super::BeginPlay();

	RandomizeNetwork();
	Test();
}

void UNeuralSystemDynamicsComponent::RandomizeNetwork()
{
	EnsureNetworkArrays();

	FRandomStream Stream(FDateTime::Now().GetTicks());
	for (float& Weight : HiddenWeights)
	{
		Weight = Stream.FRandRange(-1.0f, 1.0f);
	}

	for (float& Bias : HiddenBiases)
	{
		Bias = Stream.FRandRange(-1.0f, 1.0f);
	}

	for (float& Weight : OutputWeights)
	{
		Weight = Stream.FRandRange(-1.0f, 1.0f);
	}

	OutputBias = Stream.FRandRange(-1.0f, 1.0f);
}

float UNeuralSystemDynamicsComponent::Predict(float Time, float DampingRatio, float NaturalFrequency) const
{
	if (HiddenNeuronCount <= 0)
	{
		return 0.0f;
	}

	const float Inputs[3] = { Time, DampingRatio, NaturalFrequency };
	float Output = OutputBias;

	for (int32 NeuronIdx = 0; NeuronIdx < HiddenNeuronCount; ++NeuronIdx)
	{
		const int32 WeightIdx = NeuronIdx * 3;
		float Sum = 0.0f;

		for (int32 InputIdx = 0; InputIdx < 3; ++InputIdx)
		{
			const int32 ArrayIndex = WeightIdx + InputIdx;
			if (HiddenWeights.IsValidIndex(ArrayIndex))
			{
				Sum += HiddenWeights[ArrayIndex] * Inputs[InputIdx];
			}
		}

		if (HiddenBiases.IsValidIndex(NeuronIdx))
		{
			Sum += HiddenBiases[NeuronIdx];
		}

		const float ActivationValue = Activation(Sum);
		if (OutputWeights.IsValidIndex(NeuronIdx))
		{
			Output += OutputWeights[NeuronIdx] * ActivationValue;
		}
	}

	return Output;
}

void UNeuralSystemDynamicsComponent::Train(const TArray<FVector>& Inputs, const TArray<float>& Outputs, int32 Epochs)
{
	if (Inputs.Num() != Outputs.Num() || Inputs.Num() == 0 || Epochs <= 0)
	{
		return;
	}

	EnsureNetworkArrays();

	for (int32 Epoch = 0; Epoch < Epochs; ++Epoch)
	{
		float TotalError = 0.0f;
		TArray<float> HiddenActivations;
		HiddenActivations.SetNumZeroed(HiddenNeuronCount);
		for (int32 SampleIdx = 0; SampleIdx < Inputs.Num(); ++SampleIdx)
		{
			const FVector& Sample = Inputs[SampleIdx];
			const float InputArray[3] = { Sample.X, Sample.Y, Sample.Z };

			for (int32 NeuronIdx = 0; NeuronIdx < HiddenNeuronCount; ++NeuronIdx)
			{
				const int32 WeightIdx = NeuronIdx * 3;
				float Sum = 0.0f;

				for (int32 InputIdx = 0; InputIdx < 3; ++InputIdx)
				{
					const int32 ArrayIndex = WeightIdx + InputIdx;
					if (HiddenWeights.IsValidIndex(ArrayIndex))
					{
						Sum += HiddenWeights[ArrayIndex] * InputArray[InputIdx];
					}
				}

				if (HiddenBiases.IsValidIndex(NeuronIdx))
				{
					Sum += HiddenBiases[NeuronIdx];
				}

				HiddenActivations[NeuronIdx] = Activation(Sum);
			}

			float Output = OutputBias;
			for (int32 NeuronIdx = 0; NeuronIdx < HiddenNeuronCount; ++NeuronIdx)
			{
				if (OutputWeights.IsValidIndex(NeuronIdx))
				{
					Output += OutputWeights[NeuronIdx] * HiddenActivations[NeuronIdx];
				}
			}

			const float Target = Outputs[SampleIdx];
			const float Error = Output - Target;
			TotalError += Error * Error;

			for (int32 NeuronIdx = 0; NeuronIdx < HiddenNeuronCount; ++NeuronIdx)
			{
				const float HiddenValue = HiddenActivations[NeuronIdx];
				const float HiddenDerivative = HiddenValue * (1.0f - HiddenValue);
				const float OutputWeight = OutputWeights.IsValidIndex(NeuronIdx) ? OutputWeights[NeuronIdx] : 0.0f;
				const float DeltaHidden = Error * OutputWeight * HiddenDerivative;

				const int32 WeightIdx = NeuronIdx * 3;
				for (int32 InputIdx = 0; InputIdx < 3; ++InputIdx)
				{
					const int32 ArrayIndex = WeightIdx + InputIdx;
					if (HiddenWeights.IsValidIndex(ArrayIndex))
					{
						HiddenWeights[ArrayIndex] -= LearningRate * DeltaHidden * InputArray[InputIdx];
					}
				}

				if (HiddenBiases.IsValidIndex(NeuronIdx))
				{
					HiddenBiases[NeuronIdx] -= LearningRate * DeltaHidden;
				}
			}

			for (int32 NeuronIdx = 0; NeuronIdx < HiddenNeuronCount; ++NeuronIdx)
			{
				if (OutputWeights.IsValidIndex(NeuronIdx))
				{
					OutputWeights[NeuronIdx] -= LearningRate * Error * HiddenActivations[NeuronIdx];
				}
			}

			OutputBias -= LearningRate * Error;
		}

		if (Epoch % 100 == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Epoch %d/%d: MSE=%0.6f"), Epoch + 1, Epochs, TotalError / Inputs.Num());
		}
	}
}

void UNeuralSystemDynamicsComponent::Test()
{
	TArray<FVector> SampleInputs;
	SampleInputs.Add(FVector(0.0f, 0.1f, 2.0f));
	SampleInputs.Add(FVector(0.1f, 0.1f, 2.0f));
	SampleInputs.Add(FVector(0.2f, 0.1f, 2.0f));

	TArray<float> SampleOutputs;
	SampleOutputs.Add(1.0f);
	SampleOutputs.Add(0.9f);
	SampleOutputs.Add(0.8f);

	Train(SampleInputs, SampleOutputs, 1000);
	LastPrediction = Predict(0.3f, 0.1f, 2.0f);
	UE_LOG(LogTemp, Log, TEXT("Predicted Displacement: %0.4f"), LastPrediction);
}

void UNeuralSystemDynamicsComponent::EnsureNetworkArrays()
{
	const int32 TotalHiddenWeights = HiddenNeuronCount * 3;
	if (HiddenWeights.Num() != TotalHiddenWeights)
	{
		HiddenWeights.SetNumZeroed(TotalHiddenWeights);
	}

	if (HiddenBiases.Num() != HiddenNeuronCount)
	{
		HiddenBiases.SetNumZeroed(HiddenNeuronCount);
	}

	if (OutputWeights.Num() != HiddenNeuronCount)
	{
		OutputWeights.SetNumZeroed(HiddenNeuronCount);
	}
}

float UNeuralSystemDynamicsComponent::Activation(float Value) const
{
	return 1.0f / (1.0f + FMath::Exp(-Value));
}
