#include "PhenologicalNeuralModelComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UPhenologicalNeuralModelComponent::UPhenologicalNeuralModelComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPhenologicalNeuralModelComponent::BeginPlay()
{
	Super::BeginPlay();
	RandomizeNetwork();
	TestPrediction();
}

float UPhenologicalNeuralModelComponent::Predict(float GDD) const
{
	if (HiddenNeurons <= 0)
	{
		return 0.0f;
	}

	float SumOutput = OutputBias;
	for (int32 NeuronIndex = 0; NeuronIndex < HiddenNeurons; ++NeuronIndex)
	{
		const int32 WeightIndex = NeuronIndex;
		float HiddenSum = (HiddenWeights.IsValidIndex(WeightIndex) ? HiddenWeights[WeightIndex] * GDD : 0.0f)
			+ (HiddenBiases.IsValidIndex(NeuronIndex) ? HiddenBiases[NeuronIndex] : 0.0f);
		const float ActivationValue = Activation(HiddenSum);
		if (OutputWeights.IsValidIndex(NeuronIndex))
		{
			SumOutput += OutputWeights[NeuronIndex] * ActivationValue;
		}
	}

	return SumOutput;
}

void UPhenologicalNeuralModelComponent::Train(const TArray<float>& GDDs, const TArray<float>& Targets, int32 Epochs, float LearningRate)
{
	if (GDDs.Num() != Targets.Num() || GDDs.Num() == 0 || Epochs <= 0)
	{
		return;
	}

	EnsureNetwork();

	for (int32 Epoch = 0; Epoch < Epochs; ++Epoch)
	{
		float TotalError = 0.0f;
		for (int32 Index = 0; Index < GDDs.Num(); ++Index)
		{
			const float Input = GDDs[Index];
			const float Target = Targets[Index];
			TArray<float> HiddenOutputs;
			HiddenOutputs.SetNumZeroed(HiddenNeurons);

			for (int32 NeuronIndex = 0; NeuronIndex < HiddenNeurons; ++NeuronIndex)
			{
				const int32 WeightIndex = NeuronIndex;
				float HiddenInput = (HiddenWeights.IsValidIndex(WeightIndex) ? HiddenWeights[WeightIndex] * Input : 0.0f)
					+ (HiddenBiases.IsValidIndex(NeuronIndex) ? HiddenBiases[NeuronIndex] : 0.0f);
				HiddenOutputs[NeuronIndex] = Activation(HiddenInput);
			}

			float Output = OutputBias;
			for (int32 NeuronIndex = 0; NeuronIndex < HiddenNeurons; ++NeuronIndex)
			{
				if (OutputWeights.IsValidIndex(NeuronIndex))
				{
					Output += OutputWeights[NeuronIndex] * HiddenOutputs[NeuronIndex];
				}
			}

			const float Error = Output - Target;
			TotalError += FMath::Square(Error);
			const float OutputGradient = Error;

			for (int32 NeuronIndex = 0; NeuronIndex < HiddenNeurons; ++NeuronIndex)
			{
				if (OutputWeights.IsValidIndex(NeuronIndex))
				{
					OutputWeights[NeuronIndex] -= LearningRate * OutputGradient * HiddenOutputs[NeuronIndex];
				}
			}
			OutputBias -= LearningRate * OutputGradient;

			for (int32 NeuronIndex = 0; NeuronIndex < HiddenNeurons; ++NeuronIndex)
			{
				const float HiddenValue = HiddenOutputs[NeuronIndex];
				const float HiddenDerivative = HiddenValue * (1.0f - HiddenValue);
				const float DeltaHidden = OutputGradient * (OutputWeights.IsValidIndex(NeuronIndex) ? OutputWeights[NeuronIndex] : 0.0f) * HiddenDerivative;

				if (HiddenWeights.IsValidIndex(NeuronIndex))
				{
					HiddenWeights[NeuronIndex] -= LearningRate * DeltaHidden * Input;
				}
				if (HiddenBiases.IsValidIndex(NeuronIndex))
				{
					HiddenBiases[NeuronIndex] -= LearningRate * DeltaHidden;
				}
			}
		}

		if (Epoch % 100 == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Epoch %d/%d, Error: %0.6f"), Epoch + 1, Epochs, TotalError);
		}
	}
}

void UPhenologicalNeuralModelComponent::RandomizeNetwork()
{
	EnsureNetwork();
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

void UPhenologicalNeuralModelComponent::TestPrediction()
{
	PredictedEvent = Predict(120.0f);
	UE_LOG(LogTemp, Log, TEXT("Predicted phenological event timing for GDD=120.0: %0.4f"), PredictedEvent);
}

void UPhenologicalNeuralModelComponent::EnsureNetwork()
{
	const int32 Expected = HiddenNeurons;
	if (HiddenWeights.Num() != Expected)
	{
		HiddenWeights.SetNumZeroed(Expected);
	}
	if (HiddenBiases.Num() != Expected)
	{
		HiddenBiases.SetNumZeroed(Expected);
	}
	if (OutputWeights.Num() != Expected)
	{
		OutputWeights.SetNumZeroed(Expected);
	}
}

float UPhenologicalNeuralModelComponent::Activation(float Value) const
{
	return 1.0f / (1.0f + FMath::Exp(-Value));
}
