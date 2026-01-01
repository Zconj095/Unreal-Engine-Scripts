#include "NeuralTroubleshootingComponent.h"
#include "Logging/LogMacros.h"
#include "Math/UnrealMathUtility.h"

UNeuralTroubleshootingComponent::UNeuralTroubleshootingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNeuralTroubleshootingComponent::BeginPlay()
{
	Super::BeginPlay();

	RandomizeNetwork();
	TestPrediction();
}

float UNeuralTroubleshootingComponent::Predict(float Lambda, float Time) const
{
	if (HiddenNeuronCount <= 0)
	{
		return 0.0f;
	}

	const float Inputs[2] = { Lambda, Time };
	float SumOutput = OutputBias;

	for (int32 NeuronIdx = 0; NeuronIdx < HiddenNeuronCount; ++NeuronIdx)
	{
		const int32 WeightIndex = NeuronIdx * 2;
		float Sum = 0.0f;

		for (int32 InputIdx = 0; InputIdx < 2; ++InputIdx)
		{
			const int32 ArrayIndex = WeightIndex + InputIdx;
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
			SumOutput += OutputWeights[NeuronIdx] * ActivationValue;
		}
	}

	return SumOutput;
}

void UNeuralTroubleshootingComponent::RandomizeNetwork()
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

void UNeuralTroubleshootingComponent::TestPrediction()
{
	PredictedFaultProbability = Predict(0.5f, 2.0f);
	UE_LOG(LogTemp, Log, TEXT("Predicted Fault Identification Probability: %0.6f"), PredictedFaultProbability);
}

void UNeuralTroubleshootingComponent::EnsureNetwork()
{
	const int32 TotalHiddenWeights = HiddenNeuronCount * 2;
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

float UNeuralTroubleshootingComponent::Activation(float Value) const
{
	return 1.0f / (1.0f + FMath::Exp(-Value));
}
